#include "clgl/color.hpp"
#include <fstream>
#include <bit>
#include "clgl/utils/utils.hpp"
#include "clgl/exceptions.hpp"

clgl::Color::Color(U32 hex) {
    *this = utils::hex_to_rgb(hex);
}

void clgl::Color::set_color(U8 r, U8 g, U8 b) {
    this->r = r; this->g = g; this->b = b;
}

void clgl::Color::set_color(U32 hex) {
    *this = utils::hex_to_rgb(hex);
}

clgl::U32 clgl::Color::get_hex() const {
    return utils::rgb_to_hex(r, g, b);
}

clgl::F32 clgl::Color::calculate_luminance() const {
    return {
        _color_weights::red   * static_cast<F32>(r) * _inverse_255 +
        _color_weights::green * static_cast<F32>(g) * _inverse_255 +
        _color_weights::blue  * static_cast<F32>(b) * _inverse_255
    };
}

bool clgl::Color::operator==(const Color &other) const {
    return r == other.r && g == other.g && b == other.b;
}

bool clgl::Color::operator!=(const Color &other) const {
    return r != other.r || g != other.g || b != other.b;
}

clgl::U32 clgl::utils::rgb_to_hex(U8 r, U8 g, U8 b) {
    return (static_cast<U32>(b))      |
           (static_cast<U32>(g) << 8) |
           (static_cast<U32>(r) << 16);
}

clgl::Color clgl::utils::hex_to_rgb(U32 hex) {
    return {
        static_cast<U8>((hex & 0xFF0000) >> 16),
        static_cast<U8>((hex & 0x00FF00) >> 8),
        static_cast<U8>(hex & 0x0000FF)
    };
}

clgl::ColorMappings::~ColorMappings() {
    delete[] mp_data;
}

void clgl::ColorMappings::load() {
    std::basic_ifstream<U8> input_stream { m_data_path, std::ios_base::in | std::ios_base::binary };

    if (!input_stream.is_open()) throw exceptions::InvalidPath();

    input_stream.read(mp_data, sizeof(U8) * constants::color_space_size_24bit);
    input_stream.close();

    if (std::endian::native == std::endian::little) {
        for (U32 hex = 0u; hex < constants::color_space_size_24bit; ++hex) {
            U8 &section = mp_data[hex];

            U8 terminal_color_code = section & 0x0F;
            U8 color_brightness    = section & 0xF0;

            terminal_color_code    = utils::reverse_U8_bits(terminal_color_code) >> 4;
            color_brightness       = utils::reverse_U8_bits(color_brightness);

            section = 0u;
            section |= terminal_color_code;
            section |= (color_brightness << 4);
        }
    }
}

clgl::ColorMapping clgl::ColorMappings::get_color_mapping(Color color) const {
    return get_color_mapping(color.get_hex());
}

clgl::ColorMapping clgl::ColorMappings::get_color_mapping(U32 hex) const {
    U8 data = mp_data[hex];

    return {
        static_cast<U8>(data & 0x0F),
        static_cast<U8>((data & 0xF0) >> 4)
    };
}