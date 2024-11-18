#include "clgl/drawers/colors_16_grayscale_shaded.hpp"
#include <stdexcept>
#include <string>

void clgl::drawers::Colors16GrayscaleShaded::run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) {
    #pragma omp parallel for
    for (I32 i = 0u; i < screen_buffer.get_pixel_count(); ++i) {
        Pixel pixel = screen_buffer.get_pixel(i);

        CHAR_INFO &char_info = screen_writer.p_char_info_buffer[i];

        U8 brightness = p_color_mappings->get_color_mapping(pixel.color).brightness_value;

        U8 color = brightness >> 4;
        U8 character = color >> 2;

        char_info.Char.UnicodeChar = m_shading_palette[character];
        char_info.Attributes       = color;
    }

    screen_writer.write_char_info_buffer();
}

void clgl::drawers::Colors16GrayscaleShaded::on_set(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) {
    p_color_mappings = get_resource_manager()->access_resource<clgl::ColorMappings>(color_mappings_id);
    screen_writer.set_color_palette(constants::grayscale_color_palette);
}

void clgl::drawers::Colors16GrayscaleShaded::set_shading_palette(const std::wstring &shading_palette) {
    if (shading_palette.size() != 4) throw std::runtime_error("Invalid palette size");

    m_shading_palette = shading_palette;
}

const std::wstring &clgl::drawers::Colors16GrayscaleShaded::get_shading_palette() const {
    return m_shading_palette;
}