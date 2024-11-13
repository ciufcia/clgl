#include "clgl/color.hpp"
#include <fstream>
#include <bit>
#include "clgl/utils/utils.hpp"
#include "clgl/exceptions.hpp"

clgl::Color::Color(U32 hex, U8 a) {
    *this = utils::hex_to_rgb(hex);
    this->a = a;
}

void clgl::Color::set_color(U8 r, U8 g, U8 b, U8 a) {
    this->r = r; this->g = g; this->b = b; this->a = a;
}

void clgl::Color::set_color(U32 hex, U8 a) {
    *this = utils::hex_to_rgb(hex);
    this->a;
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
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool clgl::Color::operator!=(const Color &other) const {
    return r != other.r || g != other.g || b != other.b || a != other.a;
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

clgl::Color clgl::utils::blend_colors(Color front, Color back) {
    F32 alpha_front = static_cast<F32>(front.a) * _inverse_255;
    F32 alpha_back = static_cast<F32>(back.a) * _inverse_255;

    F32 premul_alpha_front_r = static_cast<F32>(front.r) * alpha_front;
    F32 premul_alpha_front_g = static_cast<F32>(front.g) * alpha_front;
    F32 premul_alpha_front_b = static_cast<F32>(front.b) * alpha_front;

    F32 premul_alpha_back_r  = static_cast<F32>(back.r) * alpha_back;
    F32 premul_alpha_back_g  = static_cast<F32>(back.g) * alpha_back;
    F32 premul_alpha_back_b  = static_cast<F32>(back.b) * alpha_back;

    F32 final_alpha = alpha_front + alpha_back * (1.f - alpha_front);
    F32 inverse_final_alpha = 1.f / final_alpha;

    return {
        static_cast<U8>((premul_alpha_front_r + premul_alpha_back_r * (1.f - alpha_front)) * inverse_final_alpha),
        static_cast<U8>((premul_alpha_front_g + premul_alpha_back_g * (1.f - alpha_front)) * inverse_final_alpha),
        static_cast<U8>((premul_alpha_front_b + premul_alpha_back_b * (1.f - alpha_front)) * inverse_final_alpha),
        static_cast<U8>(final_alpha * 255.f)
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