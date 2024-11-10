#include "clgl/app.hpp"

clgl::App::App(const Vec2U &screen_size, const FontData &font) {
    get_handles();

    pass_resource_manager();

    screen.init(m_handles, screen_size, font);

    input.init(m_handles);
}

void clgl::App::close() {
    screen.restore_initial_settings();
    input.restore_initial_settings();
}

void clgl::App::display() {
    screen.display();

    input.fill_input_queue();

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
    screen.mp_resource_manager = &resource_manager;
}