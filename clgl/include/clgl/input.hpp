#pragma once

#include "events.hpp"
#include <vector>

namespace clgl
{
namespace utils
{
class InputQueue
{
public:

    void pass_handle(HANDLE input_handle);

    void fill();
    void reset();

    bool grab(INPUT_RECORD &input_record);

private:

    U32                       m_front = 0u;

    std::vector<INPUT_RECORD> m_events;

    HANDLE                    m_input_handle;
};
}

class App;

class Input
{
public:

    [[nodiscard]] bool pollEvent(Event &event);
    [[nodiscard]] bool is_key_pressed(KeyCode keycode) const;

private:

    void               pass_handles(const winapi::Handles &handles);

    void               init(const winapi::Handles &handles);

    void               set_console_input_mode();
    
    void               restore_initial_settings();

    void               save_old_console_input_mode();
    void               restore_old_console_input_mode();

    void               fill_input_queue();

    void               parse_input_record(const INPUT_RECORD &input_record, Event &event) const;

    [[nodiscard]] bool process_event(const Event &event);

private:

    winapi::Handles   m_handles;

    utils::InputQueue m_input_queue;

    bool              m_key_states[static_cast<U8>(clgl::KeyCode::_KeyCodeCount)] { false };

    DWORD             m_old_console_input_mode;

friend class App;
};
}