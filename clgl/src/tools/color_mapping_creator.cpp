#include "clgl/utils/utils.hpp"
#include "clgl/color.hpp"
#include <limits>
#include <cmath>
#include <bit>
#include <fstream>
#include <string>

using namespace clgl;

std::string output_directory = "";

F32 get_squared_color_distance(Color c1, Color c2) {
    return {
        std::powf(_color_weights::red   * (static_cast<F32>(c1.r) - static_cast<F32>(c2.r)), 2) +
        std::powf(_color_weights::green * (static_cast<F32>(c1.g) - static_cast<F32>(c2.g)), 2) +
        std::powf(_color_weights::blue  * (static_cast<F32>(c1.b) - static_cast<F32>(c2.b)), 2)
    };
}

U8 find_closest_terminal_color(Color color) {
    U8  nearest_color_code     = 0u;
    F32 nearest_color_distance = std::numeric_limits<F32>::max();

    for (U8 color_code = 0u; color_code < 16u; ++color_code) {
        const Color current_color = utils::terminal_16colors_rgb::colors[color_code];

        const F32 distance = get_squared_color_distance(color, current_color);

        if (distance < nearest_color_distance) {
            nearest_color_code = color_code;
            nearest_color_distance = distance;
        }
    }

    return nearest_color_code;
}

U8 get_brightness_value(Color color) {
    F32 luminance = color.calculate_luminance();
    luminance *= 16.f;
    return (static_cast<U8>(luminance));
}

#include <iostream>
void create_clcm() {
    U8 *p_data = new U8[constants::color_space_size_24bit] { 0u };

    for (U32 hex = 0u; hex < constants::color_space_size_24bit; ++hex) {
        const Color color { hex };

        U8 terminal_color_code = find_closest_terminal_color(color);
        U8 color_brightness    = std::min(static_cast<U8>(15u), get_brightness_value(color));

        if (std::endian::native == std::endian::little) {
            terminal_color_code = (utils::reverse_U8_bits(terminal_color_code) >> 4);
            color_brightness    = (utils::reverse_U8_bits(color_brightness) >> 4);
        }

        U8 &section = p_data[hex];

        section |= terminal_color_code;
        section |= (color_brightness << 4);
    }

    std::basic_ofstream<U8> output_stream { output_directory + "color_mappings.clcm", std::ios_base::out | std::ios_base::binary };

    if (!output_stream.is_open()) throw std::runtime_error("Couldn\'t open output file for writing");

    output_stream.write(p_data, sizeof(U8) * constants::color_space_size_24bit);
    output_stream.close();
}

int main() {
    create_clcm();
    return 0;
} 