#pragma once

#include "screen.hpp"
#include "input.hpp"
#include "timings.hpp"

namespace clgl
{
class App
{
public:

                           App(const Vec2U &screen_size, const FontData &font);

    void                   close();

    void                   display();

    template<typename TimeType>
    [[nodiscard]] TimeType get_last_frame_time() const;

    void                   set_maximum_fps(U32 fps);

public:

    Screen screen;

    Input  input;

private:

    void get_handles();

private:

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