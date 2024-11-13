#pragma once

#include "screen.hpp"
#include "input.hpp"
#include "timings.hpp"
#include "clgl_resource_manager.hpp"

namespace clgl
{
class App
{
public:

                           App(const Vec2U &screen_size, const FontData &font);

    Screen &               get_screen();
    Input  &               get_input();
    CLGLResourceManager &  get_resource_manager();

    void                   close();

    void                   display();

    template<typename TimeType>
    [[nodiscard]] TimeType get_last_frame_time() const;

    void                   set_maximum_fps(U32 fps);

private:

    void get_handles();

    void pass_resource_manager();

private:

    Screen              m_screen;
    Input               m_input;
    CLGLResourceManager m_resource_manager;

    winapi::Handles m_handles;

    Clock           m_clock;

    U32             m_last_frame_time = 0u;
    U32             m_max_frame_time = 0u;
};

template<typename TimeType>
TimeType App::get_last_frame_time() const {
    utils::check_if_valid_timing_type<TimeType>();

    return std::chrono::duration_cast<TimeType>(Microseconds(m_last_frame_time));
}
}