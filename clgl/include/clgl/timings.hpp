#pragma once

#include <chrono>
#include "typedefs.hpp"

namespace clgl
{
typedef std::chrono::microseconds Microseconds;
typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::seconds      Seconds;

namespace utils
{
template<typename T>
constexpr void check_if_valid_timing_type() {
    static_assert(
        std::is_same<T, Microseconds>() |
        std::is_same<T, Milliseconds>() |
        std::is_same<T, Seconds>(),

        "Invalid timing type passed"
    );
}
}

class Clock
{
public:

    template<typename T>
    U32 time_passed() const {
        utils::check_if_valid_timing_type<T>();

        const auto now = std::chrono::steady_clock::now();

        return std::chrono::duration_cast<T>(now - m_last_restart_time).count();
    }

    template<typename T>
    U32 restart() {
        utils::check_if_valid_timing_type<T>();

        const auto now      = std::chrono::steady_clock::now();
        const U32 duration  = std::chrono::duration_cast<T>(now - m_last_restart_time).count();
        m_last_restart_time = now;

        return duration;
    }

private:

    std::chrono::steady_clock::time_point m_last_restart_time = std::chrono::steady_clock::now();
};

/**
 *  @warning Maximum sleep time is around 70 minutes, inputting more may cause overflow and a very short sleep time instead
 *  @todo Make the command not so CPU intensive when sleeping
 */
template<typename T>
void sleep(U32 value) {
    utils::check_if_valid_timing_type<T>();

    const U32                                   sleep_time = std::chrono::duration_cast<std::chrono::microseconds>(T(value)).count();
    const std::chrono::steady_clock::time_point start      = std::chrono::steady_clock::now();

    while (true) {
        const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() >= sleep_time) break;
    }
}
}