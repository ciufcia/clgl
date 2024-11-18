#include "clgl/screen_writer.hpp"
#include "clgl/constants.hpp"
#include "clgl/exceptions.hpp"

clgl::ScreenWriter::~ScreenWriter() {
    delete[] p_char_info_buffer;
}

void clgl::ScreenWriter::write_char_info_buffer() {
    WriteConsoleOutputW(
        m_output_handle,
        p_char_info_buffer,
        m_buffer_size,
        { 0u, 0u },
        &m_write_region
    );
}

void clgl::ScreenWriter::write_string_buffer() {
    SetConsoleCursorPosition(
        m_output_handle,
        { 0u, 0u }
    );

    DWORD characters_written;

    WriteConsoleW(
        m_output_handle,
        string_buffer.data(),
        static_cast<DWORD>(string_buffer.size()),
        &characters_written,
        NULL
    );

    SetConsoleCursorPosition(
        m_output_handle,
        { 0u, 0u }
    );
}

void clgl::ScreenWriter::clear_string_buffer() {
    string_buffer.clear();
}

void clgl::ScreenWriter::enable_color_blending(bool value) {
    m_color_blending_enabled = value;
}

bool clgl::ScreenWriter::is_color_blending_enabled() const {
    return m_color_blending_enabled;
}

void clgl::ScreenWriter::set_color_palette(const Color *color_palette) {
    save_old_color_palette();

    CONSOLE_SCREEN_BUFFER_INFOEX console_buffer_info;
    console_buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    if (!GetConsoleScreenBufferInfoEx(
        m_output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantFetch();

    for (U32 i = 0u; i < 16; ++i) {
        Color color = color_palette[i];
        U32 bgr_hex = 0u;
        bgr_hex |= color.r;
        bgr_hex |= (color.g << 8);
        bgr_hex |= (color.b << 16);

        console_buffer_info.ColorTable[i] = static_cast<DWORD>(bgr_hex);
    }

    if (!SetConsoleScreenBufferInfoEx(
        m_output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantSet();
}

void clgl::ScreenWriter::save_old_color_palette() {
    if (m_first_palette_set) return;
    m_first_palette_set = true;

    CONSOLE_SCREEN_BUFFER_INFOEX console_buffer_info;
    console_buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    if (!GetConsoleScreenBufferInfoEx(
        m_output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantFetch();

    std::copy(console_buffer_info.ColorTable, console_buffer_info.ColorTable + 16, m_old_color_palette);
}

void clgl::ScreenWriter::restore_old_color_palette() {
    if (!m_first_palette_set) return;

    CONSOLE_SCREEN_BUFFER_INFOEX console_buffer_info;
    console_buffer_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    if (!GetConsoleScreenBufferInfoEx(
        m_output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantFetch();

    std::copy(m_old_color_palette, m_old_color_palette + 16, console_buffer_info.ColorTable);

    if (!SetConsoleScreenBufferInfoEx(
        m_output_handle,
        &console_buffer_info
    )) throw exceptions::winapi::CantSet();

    m_first_palette_set = false;
}

void clgl::ScreenWriter::set_size(const Vec2U &size) {
    if (size.x == 0u || size.y == 0u) throw exceptions::InvalidParameter();

    m_buffer_size = COORD{ static_cast<SHORT>(size.x), static_cast<SHORT>(size.y) };
    m_write_region = { 0u, 0u, static_cast<SHORT>(m_buffer_size.X - 1), static_cast<SHORT>(m_buffer_size.Y - 1) };

    delete[] p_char_info_buffer;
    p_char_info_buffer = new CHAR_INFO[static_cast<U32>(m_buffer_size.X) * static_cast<U32>(m_buffer_size.Y)];

    string_buffer.reserve(size.x * size.y * constants::ANSI_coloring_output_length);
}