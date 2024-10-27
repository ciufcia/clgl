#include "clgl/events.hpp"

bool clgl::KeyEvent::pressed() const {
    return m_down;
}

bool clgl::KeyEvent::released() const {
    return !m_down;
}

bool clgl::KeyEvent::check_control_key(ControlKey control_key) const {
    return _CLGL_BIT_MASK(m_control_key_state, static_cast<U16>(control_key));
}

bool clgl::MouseEvent::pressed(MouseButton button) const {
    return _CLGL_BIT_MASK(m_mouse_state, static_cast<U8>(button));
}

bool clgl::MouseEvent::check_control_key(ControlKey control_key) const {
    return _CLGL_BIT_MASK(m_control_key_state, static_cast<U16>(control_key));
}

bool clgl::MouseEvent::check_flag(Flag flag) const {
    return _CLGL_BIT_MASK(m_flags, static_cast<U8>(flag));
}