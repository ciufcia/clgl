#include "clgl/utils/utils.hpp"
#include "clgl/color.hpp"
#include <limits>
#include <cmath>
#include <bit>
#include <fstream>
#include <string>
#include <iostream>
#include "color_mappings_config.hpp"

using namespace clgl;

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

    for (U32 color_code = 0u; color_code < 16u; ++color_code) {
        const Color current_color = color_mappings_creator::template_palette[color_code];

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
    luminance *= 256.f;
    return (static_cast<U8>(std::min(luminance, 255.f)));
}

void create_clcm() {
    constexpr U32 data_size = (constants::color_space_size_24bit / 2) * sizeof(U8) * 3 + constants::color_palette_size * sizeof(U8) * 3;

    U8 *p_data = new U8[data_size] { 0u };

    std::cout << "-> Saving the color palette... ";

    for (U32 i = 0u; i < constants::color_palette_size; ++i) {
        Color color = color_mappings_creator::template_palette[i];
        
        U8 r = color.r;
        U8 g = color.g;
        U8 b = color.b;

        if (std::endian::native == std::endian::little) {
            r = utils::reverse_U8_bits(r);
            g = utils::reverse_U8_bits(g);
            b = utils::reverse_U8_bits(b);
        }

        p_data[i * 3]     = r;
        p_data[i * 3 + 1] = g;
        p_data[i * 3 + 2] = b;
    }

    std::cout << "Done!\n";

    constexpr U32 color_mappings_offset = constants::color_palette_size * sizeof(U8) * 3;

    std::cout << "-> Generating color mappings... ";

    #pragma omp parallel for
    for (I32 section_index = 0; section_index < constants::color_space_size_24bit / 2; ++section_index) {
        U32 hex1 = static_cast<U32>(section_index) * 2;
        U32 hex2 = static_cast<U32>(section_index) * 2 + 1;

        const Color color1 { hex1 };
        const Color color2 { hex2 };

        U8 terminal_color_code1 = find_closest_terminal_color(color1);
        U8 color_brightness1 = get_brightness_value(color1);
        U8 terminal_color_code2 = find_closest_terminal_color(color2);
        U8 color_brightness2 = get_brightness_value(color2);

        if (std::endian::native == std::endian::little) {
            color_brightness1 = utils::reverse_U8_bits(color_brightness1);
            terminal_color_code1 = utils::reverse_U8_bits(terminal_color_code1);
            color_brightness2 = utils::reverse_U8_bits(color_brightness2);
            terminal_color_code2 = (utils::reverse_U8_bits(terminal_color_code2) >> 4);
        }

        U8 color_codes = 0u;
        color_codes |= terminal_color_code1;
        color_codes |= terminal_color_code2;

        p_data[color_mappings_offset + section_index * 3] = color_brightness1;
        p_data[color_mappings_offset + section_index * 3 + 1] = color_codes;
        p_data[color_mappings_offset + section_index * 3 + 2] = color_brightness2;
    }

    std::cout << "Done!\n";

    std::cout << "-> Writing color mappings to: \'" << color_mappings_creator::output_path << "\'... ";

    std::basic_ofstream<U8> output_stream { color_mappings_creator::output_path, std::ios_base::out | std::ios_base::binary };

    if (!output_stream.is_open()) throw std::runtime_error("Couldn\'t open output file for writing");

    output_stream.write(p_data, data_size);
    output_stream.close();

    std::cout << "Done!\n";
}

int main() {
    create_clcm();
    return 0;
} 