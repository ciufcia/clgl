#include "clgl/drawers/single_color_16_shaded.hpp"
#include <stdexcept>

void clgl::drawers::SingleColor16Shaded::run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) {
    U8 console_color = p_color_mappings->get_color_mapping(color).color_code;

    #pragma omp parallel for
    for (I32 i = 0u; i < screen_buffer.get_pixel_count(); ++i) {
        Pixel pixel = screen_buffer.get_pixel(i);

        CHAR_INFO &char_info = screen_writer.p_char_info_buffer[i];

        U8 brightness              = p_color_mappings->get_color_mapping(pixel.color).brightness_value;
        char_info.Char.UnicodeChar = m_shading_palette[brightness];
        char_info.Attributes       = console_color;
    }

    screen_writer.write_char_info_buffer();
}

void clgl::drawers::SingleColor16Shaded::set_shading_palette(const std::wstring &shading_palette) {
    if (shading_palette.size() != 16) throw std::runtime_error("Invalid palette size");

    m_shading_palette = shading_palette;
}

const std::wstring &clgl::drawers::SingleColor16Shaded::get_shading_palette() const {
    return m_shading_palette;
}

void clgl::drawers::SingleColor16Shaded::on_set() {
    p_color_mappings = get_resource_manager()->access_resource<clgl::ColorMappings>();
}