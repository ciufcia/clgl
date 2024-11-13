#pragma once

#include "utils/winapi_include.hpp"
#include "vec.hpp"
#include <string>

namespace clgl
{
class ScreenWriter
{
public:

    CHAR_INFO   *p_char_info_buffer = nullptr;
    std::wstring string_buffer;

public:

         ~ScreenWriter();

    void write_char_info_buffer();
    void write_string_buffer();

    void clear_string_buffer();

    void enable_color_blending(bool value = true);
    [[nodisacrd]] bool is_color_blending_enabled() const;

private:

    void set_size(const Vec2U &size);

    bool m_color_blending_enabled = false;

    HANDLE     m_output_handle;
    COORD      m_buffer_size;
    SMALL_RECT m_write_region;

friend class Screen;
};
}