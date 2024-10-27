#include "clgl/input.hpp"
#include "clgl/exceptions.hpp"

void clgl::utils::InputQueue::pass_handle(HANDLE input_handle) {
    m_input_handle = input_handle;
}

void clgl::utils::InputQueue::fill() {
    DWORD number_of_events;

    if (!GetNumberOfConsoleInputEvents(
        m_input_handle,
        &number_of_events
    )) throw exceptions::winapi::CantFetch();

    m_events.resize(static_cast<U32>(number_of_events));

    if (number_of_events == 0) return;

    DWORD events_read;

    ReadConsoleInput(
        m_input_handle,
        m_events.data(),
        number_of_events,
        &events_read
    );
}

void clgl::utils::InputQueue::reset() {
    m_events.clear();
    m_front = 0u;
}

bool clgl::utils::InputQueue::grab(INPUT_RECORD &input_record) {
    if (m_front >= m_events.size()) return false;

    input_record = m_events[m_front];

    ++m_front;

    return true;
}

bool clgl::Input::pollEvent(Event &event) {
    INPUT_RECORD input_record;

    while (true) {
        if (!m_input_queue.grab(input_record)) return false;

        parse_input_record(input_record, event);

        if (process_event(event)) return true;
    }
}

bool clgl::Input::is_key_pressed(KeyCode keycode) const {
    return m_key_states[static_cast<U8>(keycode)];
}

void clgl::Input::pass_handles(const winapi::Handles &handles) {
    m_handles = handles;
    m_input_queue.pass_handle(m_handles.input_handle);
}

void clgl::Input::init(const winapi::Handles &handles) {
    pass_handles(handles);
    set_console_input_mode();
}

void clgl::Input::set_console_input_mode() {
    save_old_console_input_mode();

    DWORD console_input_mode = 0u;

    console_input_mode = ENABLE_EXTENDED_FLAGS |
                         ENABLE_MOUSE_INPUT    |
                         ENABLE_WINDOW_INPUT;

    if (!SetConsoleMode(
        m_handles.input_handle,
        console_input_mode
    )) throw exceptions::winapi::CantSet();                    
}

void clgl::Input::restore_initial_settings() {
    restore_old_console_input_mode();
}

void clgl::Input::save_old_console_input_mode() {
    if (!GetConsoleMode(
        m_handles.input_handle,
        &m_old_console_input_mode
    )) throw exceptions::winapi::CantFetch();
}

void clgl::Input::restore_old_console_input_mode() {
    if (!SetConsoleMode(
        m_handles.input_handle,
        m_old_console_input_mode
    )) throw exceptions::winapi::CantSet();
}

void clgl::Input::fill_input_queue() {
    m_input_queue.reset();
    m_input_queue.fill();
}

void clgl::Input::parse_input_record(const INPUT_RECORD &input_record, Event &event) const {
    event.type = EventType(input_record.EventType);
    
    switch (event.type) {
        case EventType::Key:
            event.event.keyEvent.keycode               = KeyCode(input_record.Event.KeyEvent.wVirtualKeyCode);
            event.event.keyEvent.m_down                = input_record.Event.KeyEvent.bKeyDown;
            event.event.keyEvent.m_control_key_state   = static_cast<U16>(input_record.Event.KeyEvent.dwControlKeyState);
            break;
        case EventType::Mouse:
            COORD mouse_position                       = input_record.Event.MouseEvent.dwMousePosition;
            event.event.mouseEvent.position            = { static_cast<I32>(mouse_position.X), static_cast<I32>(mouse_position.Y) };
            event.event.mouseEvent.m_mouse_state       = static_cast<U8>(input_record.Event.MouseEvent.dwButtonState);
            event.event.mouseEvent.m_control_key_state = static_cast<U16>(input_record.Event.MouseEvent.dwControlKeyState);
            event.event.mouseEvent.m_flags             = static_cast<U8>(input_record.Event.MouseEvent.dwEventFlags);
            break;
        case EventType::Console:
            COORD new_size                             = input_record.Event.WindowBufferSizeEvent.dwSize; 
            event.event.consoleEvent.new_size          = { static_cast<U32>(new_size.X), static_cast<U32>(new_size.Y) };
            break;
    }
}

bool clgl::Input::process_event(const Event &event) {
    if (event.type == EventType::Key) {
        U8 key_code = static_cast<U8>(event.event.keyEvent.keycode);
        if (event.event.keyEvent.m_down) {
            if (m_key_states[key_code]) return false;
            else                        m_key_states[key_code] = true;    
        } else {
            m_key_states[key_code] = false;
        }
    }

    return true;
}