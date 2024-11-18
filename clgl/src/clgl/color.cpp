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

void clgl::ColorMappings::load(const std::string &file_path) {
    std::basic_ifstream<U8> input_stream { file_path, std::ios_base::in | std::ios_base::binary };

    if (!input_stream.is_open()) throw exceptions::InvalidPath();

    constexpr U32 data_size = (constants::color_space_size_24bit / 2) * sizeof(U8) * 3 + constants::color_palette_size * sizeof(U8) * 3;
    U8 *p_data = new U8[data_size] { 0u };

    delete[] mp_data;
    mp_data = new U8[(constants::color_space_size_24bit / 2) * sizeof(U8) * 3];

    input_stream.read(p_data, data_size);
    input_stream.close();

    for (U32 i = 0u; i < constants::color_palette_size; ++i) {
        Color &color = m_color_palette[i];
        color.r = p_data[i * 3];
        color.g = p_data[i * 3 + 1];
        color.b = p_data[i * 3 + 2];

        if (std::endian::native == std::endian::little) {
            color.r = utils::reverse_U8_bits(color.r);
            color.g = utils::reverse_U8_bits(color.g);
            color.b = utils::reverse_U8_bits(color.b);
        }
    }

    constexpr U32 color_mappings_offset = constants::color_palette_size * sizeof(U8) * 3;

    #pragma omp parallel for
    for (I32 section_index = 0; section_index < constants::color_space_size_24bit / 2; ++section_index) {
        U8 brightness1 = p_data[color_mappings_offset + section_index * 3];
        U8 color_codes = p_data[color_mappings_offset + section_index * 3 + 1];
        U8 brightness2 = p_data[color_mappings_offset + section_index * 3 + 2];

        U32 hex1 = section_index * 2;

        if (std::endian::native == std::endian::little) {
            clgl::U8 color_code1 = (color_codes & 0x0F) << 4;
            clgl::U8 color_code2 = color_codes & 0xF0;

            color_code1 = clgl::utils::reverse_U8_bits(color_code1);
            color_code2 = clgl::utils::reverse_U8_bits(color_code2) << 4;

            color_codes = 0u;
            color_codes |= (color_code1);
            color_codes |= (color_code2);

            brightness1 = utils::reverse_U8_bits(brightness1);
            brightness2 = utils::reverse_U8_bits(brightness2);
        }

        mp_data[section_index * 3] = brightness1;
        mp_data[section_index * 3 + 1] = color_codes;
        mp_data[section_index * 3 + 2] = brightness2;
    }
}

const clgl::Color *clgl::ColorMappings::get_color_palette() const {
    return m_color_palette;
}

clgl::ColorMapping clgl::ColorMappings::get_color_mapping(Color color) const {
    return get_color_mapping(color.get_hex());
}

clgl::ColorMapping clgl::ColorMappings::get_color_mapping(U32 hex) const {
    ColorMapping color_mapping;

    U32 section_index = (hex >> 1);
    U32 data_index = section_index * 3;

    // number is not even
    if (_CLGL_CHECK_BIT(hex, 0)) {
        color_mapping.color_code = (mp_data[data_index + 1] & 0xF0) >> 4;
        color_mapping.brightness_value = mp_data[data_index + 2];
    } else {
        color_mapping.color_code = mp_data[data_index + 1] & 0x0F;
        color_mapping.brightness_value = mp_data[data_index];
    }

    return color_mapping;
}