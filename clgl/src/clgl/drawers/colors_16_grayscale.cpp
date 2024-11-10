#include "clgl/drawers/colors_16_grayscale.hpp"

void clgl::drawers::Colors16Grayscale::run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) {
    #pragma omp parallel for
    for (I32 i = 0u; i < screen_buffer.get_pixel_count(); ++i) {
        const Pixel &pixel = screen_buffer.get_pixel(i);

        CHAR_INFO &char_info = screen_writer.p_char_info_buffer[i];

        char_info.Char.UnicodeChar = pixel.character;
        U8 brightness              = p_color_mappings->get_color_mapping(pixel.color).brightness_value;
        char_info.Attributes       = m_grayscale_colors[brightness >> 2];
    }

    screen_writer.write_char_info_buffer();
}

void clgl::drawers::Colors16Grayscale::on_set() {
    p_color_mappings = get_resource_manager()->access_resource<clgl::ColorMappings>();
}