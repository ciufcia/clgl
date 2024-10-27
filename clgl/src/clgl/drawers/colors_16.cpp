#include "clgl/drawers/colors_16.hpp"

void clgl::drawers::Colors16::run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer, const ColorMappings &color_mappings) {
    for (U32 i = 0u; i < screen_buffer.get_pixel_count(); ++i) {
        const Pixel &pixel = screen_buffer.get_pixel(i);

        CHAR_INFO &char_info = screen_writer.p_char_info_buffer[i];

        char_info.Char.UnicodeChar = pixel.character;
        char_info.Attributes       = color_mappings.get_color_mapping(pixel.color).color_code;
    }

    screen_writer.write_char_info_buffer();
}