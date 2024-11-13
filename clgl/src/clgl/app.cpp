#include "clgl/app.hpp"

clgl::App::App(const Vec2U &screen_size, const FontData &font) {
    get_handles();

    pass_resource_manager();

    m_screen.init(m_handles, screen_size, font);

    m_input.init(m_handles);
}

clgl::Screen &clgl::App::get_screen() {
    return m_screen;
}

clgl::Input &clgl::App::get_input() {
    return m_input;
}

clgl::CLGLResourceManager &clgl::App::get_resource_manager() {
    return m_resource_manager;
}

void clgl::App::close() {
    m_screen.restore_initial_settings();
    m_input.restore_initial_settings();
}

void clgl::App::display() {
    m_screen.display();

    m_input.fill_input_queue();

    m_last_frame_time = m_clock.time_passed<clgl::Microseconds>();

    if (m_last_frame_time < m_max_frame_time) {
        clgl::U32 sleep_time = m_max_frame_time - m_last_frame_time;
        clgl::sleep<clgl::Microseconds>(sleep_time);
    }

    m_last_frame_time = m_clock.restart<clgl::Microseconds>();
}

void clgl::App::set_maximum_fps(U32 fps) {
    m_max_frame_time = static_cast<U32>((1.f / 60.f) * 1000000.f);
}

void clgl::App::get_handles() {
    m_handles.output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    m_handles.input_handle  = GetStdHandle(STD_INPUT_HANDLE);
    m_handles.window_handle = GetConsoleWindow();
}

void clgl::App::pass_resource_manager() {
    m_screen.mp_resource_manager = &m_resource_manager;
}