#include "clgl/screen.hpp"
#include <stdexcept>
#include <thread>
#include <iostream>
#include "clgl/drawers/drawers.hpp"

void clgl::Screen::fill(const Pixel &pixel) {
    const U32 range_size = m_screen_buffer.get_pixel_count() / _SCREEN_FILLING_THREADS_COUNT;

    constexpr U32 threads_running = _SCREEN_FILLING_THREADS_COUNT - 1u;

    U32 current_start = 0u;
    U32 current_end   = range_size;

    std::thread filling_threads[_SCREEN_FILLING_THREADS_COUNT];

    for (U32 i = 0u; i < threads_running; ++i) {
        filling_threads[i] = std::thread(&Screen::fill_screen_portion, this, current_start, current_end, pixel);

        current_start = current_end;
        current_end   += range_size;
    }

    filling_threads[threads_running] = std::thread(&Screen::fill_screen_portion, this, current_start, m_screen_buffer.get_pixel_count(), pixel);

    for (U32 i = 0u; i < _SCREEN_FILLING_THREADS_COUNT; ++i) {
        filling_threads[i].join();
    }
}

void clgl::Screen::draw(Drawable &drawable) {
    drawable.draw(m_screen_buffer);
}

void clgl::Screen::draw_no_clipping(Drawable &drawable) {
    drawable.draw_no_clipping(m_screen_buffer);
}

const clgl::Vec2U &clgl::Screen::get_size() const {
    return m_size;
}

void clgl::Screen::set_size(const Vec2U &size) {
    save_old_size();

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    if (!GetConsoleScreenBufferInfo(
        m_handles.output_handle,
        &screen_buffer_info
    )) throw exceptions::winapi::CantFetch();

    COORD largest_console_window_size = GetLargestConsoleWindowSize(m_handles.output_handle);
    if (largest_console_window_size.X + largest_console_window_size.Y == 0) throw exceptions::winapi::CantFetch();

    // The size of the console buffer can be bigger than the maximum allowed size of the console buffer somehow???
    // This fixes it
    if (screen_buffer_info.dwSize.X > largest_console_window_size.X || screen_buffer_info.dwSize.Y > largest_console_window_size.Y) {
        FontData old_font = m_current_font_data;
        FontData new_font { old_font.face_name, old_font.size };

        if (screen_buffer_info.dwSize.X > largest_console_window_size.X) {
            new_font.size.x = (old_font.size.x * static_cast<U32>(largest_console_window_size.X)) / static_cast<U32>(screen_buffer_info.dwSize.X);
            if (new_font.size.x == 0u) throw exceptions::InvalidParameter();
        }

        if (screen_buffer_info.dwSize.Y > largest_console_window_size.Y) {
            new_font.size.y = (old_font.size.y * static_cast<U32>(largest_console_window_size.Y)) / static_cast<U32>(screen_buffer_info.dwSize.Y);
            if (new_font.size.y == 0u) throw exceptions::InvalidParameter();
        }

        set_font(new_font);
        set_size(Vec2U(largest_console_window_size.X, largest_console_window_size.Y));
        set_font(old_font);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const SMALL_RECT max_window_rect { 0u, 0u, largest_console_window_size.X - 1, largest_console_window_size.Y - 1 };

    if (static_cast<SHORT>(size.x) > largest_console_window_size.X || static_cast<SHORT>(size.y) > largest_console_window_size.Y) throw exceptions::InvalidParameter();

    if (!SetConsoleScreenBufferSize(
        m_handles.output_handle,
        largest_console_window_size
    )) throw exceptions::winapi::CantSet();

    if (!GetConsoleScreenBufferInfo(
        m_handles.output_handle,
        &screen_buffer_info
    )) throw exceptions::winapi::CantFetch();

    const COORD new_window_size { static_cast<SHORT>(size.x), static_cast<SHORT>(size.y) };
    const COORD min_window_size { static_cast<SHORT>(GetSystemMetrics(SM_CXMIN)), static_cast<SHORT>(GetSystemMetrics(SM_CYMIN)) };

    if (new_window_size.X * static_cast<SHORT>(m_current_font_data.size.x) < min_window_size.X ||
        new_window_size.Y * static_cast<SHORT>(m_current_font_data.size.y) < min_window_size.Y) throw exceptions::InvalidParameter();

    const SMALL_RECT new_window_rect { 0u, 0u, new_window_size.X - 1, new_window_size.Y - 1 };

    if (!SetConsoleWindowInfo(
        m_handles.output_handle,
        TRUE,
        &new_window_rect
    )) throw exceptions::winapi::CantSet();

    if (!SetConsoleScreenBufferSize(
        m_handles.output_handle,
        new_window_size
    )) throw exceptions::winapi::CantSet();

    m_size = size;
    m_screen_buffer.set_size(size);
    m_screen_writer.set_size(size);
}

void clgl::Screen::set_font(const FontData &font_data) {
    if (font_data.size.x == 0u || font_data.size.y == 0u) throw std::runtime_error("Invalid font size");

    save_old_font();

    CONSOLE_FONT_INFOEX font_info;
    font_info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    font_info.dwFontSize = COORD{static_cast<SHORT>(font_data.size.x), static_cast<SHORT>(font_data.size.y)};
    font_info.FontWeight = font_data.weight;
    font_info.FontFamily = font_data.family;
    wcscpy_s(font_info.FaceName, font_data.face_name.c_str());

    if (!SetCurrentConsoleFontEx(
        m_handles.output_handle,
        FALSE,
        &font_info
    )) throw exceptions::winapi::CantSet();

    m_current_font_data = font_data;
}

void clgl::Screen::set_title(const std::wstring &title) {
    if (!SetConsoleTitleW(
        title.data()
    )) throw exceptions::winapi::CantSet();
}

std::shared_ptr<clgl::Drawer> clgl::Screen::get_drawer(U32 id) {
    if (id >= m_drawers.size()) throw exceptions::InvalidParameter();

    return m_drawers[id];
}

void clgl::Screen::set_drawer(U32 id) {
    if (id >= m_drawers.size()) throw exceptions::InvalidParameter();

    m_current_drawer_id = id;
    mp_current_drawer   = m_drawers[id];
}

clgl::U32 clgl::Screen::get_current_drawer_id() const {
    return m_current_drawer_id;
}

void clgl::Screen::init(const winapi::Handles &handles, const Vec2U &size, const FontData &font_data) {
    pass_handles(handles);
    load_color_mappings();
    set_console_output_mode();
    hide_cursor();
    set_color_palette();

    set_font(font_data);
    set_size(size);

    register_and_set_drawer<drawers::FullRGB>();
}

void clgl::Screen::display() {
    mp_current_drawer->run(m_screen_buffer, m_screen_writer, m_color_mappings);
}

void clgl::Screen::pass_handles(const winapi::Handles &handles) {
    m_handles = handles;

    m_screen_writer.m_output_handle = handles.output_handle;
}

void clgl::Screen::load_color_mappings() {
    m_color_mappings.load();
}

void clgl::Screen::set_console_output_mode() {
    save_old_console_output_mode();

    const DWORD console_output_mode = ENABLE_PROCESSED_OUTPUT            |
                                      ENABLE_VIRTUAL_TERMINAL_PROCESSING |
                                      DISABLE_NEWLINE_AUTO_RETURN;

    if (!SetConsoleMode(
        m_handles.output_handle,
        console_output_mode
    )) throw exceptions::winapi::CantSet();
}

void clgl::Screen::hide_cursor() {
    save_old_cursor_info();

    CONSOLE_CURSOR_INFO console_cursor_info;

    if (!GetConsoleCursorInfo(
        m_handles.output_handle,
        &console_cursor_info
    )) throw exceptions::winapi::CantFetch();

    console_cursor_info.bVisible = 0u;

    if (!SetConsoleCursorInfo(
        m_handles.output_handle,
        &console_cursor_info
    )) throw exceptions::winapi::CantSet();
}

void clgl::Screen::set_color_palette() {
    save_old_color_palette();

    CONSOLE_SCREEN_BUFFER_INFOEX console_buffer_info;
    console_buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    if (!GetConsoleScreenBufferInfoEx(
        m_handles.output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantFetch();

    for (U32 i = 0u; i < 16; ++i) {
        Color color = clgl::utils::terminal_16colors_rgb::colors[i];
        U32 bgr_hex = 0u;
        bgr_hex |= color.r;
        bgr_hex |= (color.g << 8);
        bgr_hex |= (color.b << 16);

        console_buffer_info.ColorTable[i] = static_cast<DWORD>(bgr_hex);
    }

    if (!SetConsoleScreenBufferInfoEx(
        m_handles.output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantSet();
}

void clgl::Screen::restore_initial_settings() {
    std::cout << "\033[2J";

    restore_old_font();
    restore_old_size();
    restore_old_cursor_info();
    restore_old_color_palette();

    restore_old_console_output_mode();
}

void clgl::Screen::save_old_console_output_mode() {
    if (!GetConsoleMode(
        m_handles.output_handle,
        &m_old_console_output_mode
    )) throw exceptions::winapi::CantFetch();
}

void clgl::Screen::restore_old_console_output_mode() {
    if (!SetConsoleMode(
        m_handles.output_handle,
        m_old_console_output_mode
    )) throw exceptions::winapi::CantSet();
}

void clgl::Screen::save_old_font() {
    if (m_first_font_set) return;

    m_old_font_info.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    if (!GetCurrentConsoleFontEx(
        m_handles.output_handle,
        FALSE,
        &m_old_font_info
    )) throw exceptions::winapi::CantFetch();

    m_first_font_set = true;
}

void clgl::Screen::restore_old_font() {
    if (!SetCurrentConsoleFontEx(
        m_handles.output_handle,
        FALSE,
        &m_old_font_info
    )) throw exceptions::winapi::CantSet();

    m_first_font_set = false;
}

void clgl::Screen::save_old_size() {
    if (m_first_size_set) return;

    CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
    if (!GetConsoleScreenBufferInfo(
        m_handles.output_handle,
        &console_screen_buffer_info
    )) throw exceptions::winapi::CantFetch();

    m_old_size = Vec2U{ static_cast<U32>(console_screen_buffer_info.dwSize.X), static_cast<U32>(console_screen_buffer_info.dwSize.Y) };

    m_first_size_set = true;
}

void clgl::Screen::restore_old_size() {
    try { set_size(m_old_size); } catch (exceptions::InvalidParameter &exception) {}

    m_first_size_set = false;
}

void clgl::Screen::save_old_cursor_info() {
    if (!GetConsoleCursorInfo(
        m_handles.output_handle,
        &m_old_cursor_info
    )) throw exceptions::winapi::CantFetch();
}

void clgl::Screen::restore_old_cursor_info() {
    if (!SetConsoleCursorInfo(
        m_handles.output_handle,
        &m_old_cursor_info
    )) throw exceptions::winapi::CantSet();
}

void clgl::Screen::save_old_color_palette() {
    CONSOLE_SCREEN_BUFFER_INFOEX console_buffer_info;
    console_buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    if (!GetConsoleScreenBufferInfoEx(
        m_handles.output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantFetch();

    std::copy(console_buffer_info.ColorTable, console_buffer_info.ColorTable + 16, m_old_color_palette);
}

void clgl::Screen::restore_old_color_palette() {
    CONSOLE_SCREEN_BUFFER_INFOEX console_buffer_info;
    console_buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    if (!GetConsoleScreenBufferInfoEx(
        m_handles.output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantFetch();

    std::copy(m_old_color_palette, m_old_color_palette + 16, console_buffer_info.ColorTable);

    if (!SetConsoleScreenBufferInfoEx(
        m_handles.output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantSet();
}

void clgl::Screen::fill_screen_portion(U32 range_start, U32 range_end, const Pixel &pixel) {
    for (U32 p = range_start; p < range_end; ++p) {
        m_screen_buffer.set_pixel(p, pixel);
    }
}