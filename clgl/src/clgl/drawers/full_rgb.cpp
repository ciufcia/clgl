#include "clgl/drawers/full_rgb.hpp"

void clgl::drawers::FullRGB::run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) {
    screen_writer.clear_string_buffer();

    std::wstring coloring_string = L"";

    Pixel first_pixel = screen_buffer.get_pixel(0u);
    Color current_color = first_pixel.color;
    utils::get_ANSI_color(coloring_string, current_color);
    screen_writer.string_buffer += coloring_string;
    coloring_string.clear();

    U32 current_index = 0u;

    for (U32 y = 0u; y < screen_buffer.get_size().y; ++y) {
        current_index = y * screen_buffer.get_size().x;

        for (U32 x = 0u; x < screen_buffer.get_size().x; ++x) {
            Pixel pixel = screen_buffer.get_pixel(current_index);

            if (pixel.color != current_color) {
                current_color = pixel.color;
                utils::get_ANSI_color(coloring_string, current_color);
                screen_writer.string_buffer += coloring_string;
                coloring_string.clear();
            }

            screen_writer.string_buffer += pixel.character;

            ++current_index;
        }

        screen_writer.string_buffer += constants::ANSI_NEWLINE;
    }

    utils::get_ANSI_color_reset(coloring_string);
    screen_writer.string_buffer += coloring_string;

    screen_writer.write_string_buffer();
}