#include "clgl/drawers/single_color_shaded.hpp"
#include <stdexcept>

void clgl::drawers::SingleColorShaded::run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) {
    screen_writer.clear_string_buffer();

    std::wstring coloring_string = L"";

    utils::get_ANSI_color(coloring_string, color);
    screen_writer.string_buffer += coloring_string;
    coloring_string.clear();

    U32 current_index = 0u;

    for (U32 y = 0u; y < screen_buffer.get_size().y; ++y) {
        current_index = y * screen_buffer.get_size().x;

        for (U32 x = 0u; x < screen_buffer.get_size().x; ++x) {
            Pixel pixel = screen_buffer.get_pixel(current_index);

            U8 brightness = p_color_mappings->get_color_mapping(pixel.color).brightness_value;

            screen_writer.string_buffer += m_shading_palette[brightness];

            ++current_index;
        }

        screen_writer.string_buffer += constants::ANSI_NEWLINE;
    }

    utils::get_ANSI_color_reset(coloring_string);
    screen_writer.string_buffer += coloring_string;

    screen_writer.write_string_buffer();
}

void clgl::drawers::SingleColorShaded::set_shading_palette(const std::wstring &shading_palette) {
    if (shading_palette.size() != 16) throw std::runtime_error("Invalid palette size");

    m_shading_palette = shading_palette;
}

const std::wstring &clgl::drawers::SingleColorShaded::get_shading_palette() const {
    return m_shading_palette;
}

void clgl::drawers::SingleColorShaded::on_set() {
    p_color_mappings = get_resource_manager()->access_resource<clgl::ColorMappings>();
}