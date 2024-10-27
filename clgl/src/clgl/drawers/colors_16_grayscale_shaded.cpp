#include "clgl/drawers/colors_16_grayscale_shaded.hpp"
#include <stdexcept>
#include <string>

void clgl::drawers::Colors16GrayscaleShaded::run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer, const ColorMappings &color_mappings) {
    for (U32 i = 0u; i < screen_buffer.get_pixel_count(); ++i) {
        const Pixel &pixel = screen_buffer.get_pixel(i);

        CHAR_INFO &char_info = screen_writer.p_char_info_buffer[i];

        U8 brightness = color_mappings.get_color_mapping(pixel.color).brightness_value;

        U8 color_index     = (brightness  & 0b1100) >> 2;
        U8 character_index = brightness & 0b0011;

        char_info.Char.UnicodeChar = m_shading_palette[character_index];
        char_info.Attributes       = m_grayscale_colors[color_index];
    }

    screen_writer.write_char_info_buffer();
}

void clgl::drawers::Colors16GrayscaleShaded::set_shading_palette(const std::wstring &shading_palette) {
    if (shading_palette.size() != 4) throw std::runtime_error("Invalid palette size");

    m_shading_palette = shading_palette;
}

const std::wstring &clgl::drawers::Colors16GrayscaleShaded::get_shading_palette() const {
    return m_shading_palette;
}