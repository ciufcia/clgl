#pragma once

#include "utils/winapi_include.hpp"
#include "clgl/color.hpp"
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
    [[nodiscard]] bool is_color_blending_enabled() const;

    void set_color_palette(const Color *color_palette);
    void save_old_color_palette();
    void restore_old_color_palette();

private:

    void set_size(const Vec2U &size);

    bool m_color_blending_enabled = false;

    HANDLE     m_output_handle;
    COORD      m_buffer_size;
    SMALL_RECT m_write_region;

    COLORREF   m_old_color_palette[16];
    bool       m_first_palette_set = false;

friend class Screen;
};
}