#include "clgl/texture.hpp"
#include "clgl/../external/stb_image.h"
#include <stdexcept>
#include "clgl/exceptions.hpp"
#include <cmath>
#include <algorithm>
#include "clgl/utils/winapi_include.hpp"

clgl::Texture::~Texture() {
    delete[] mp_pixels;
}

void clgl::Texture::load(const std::string &filename, Character character) {
    delete[] mp_pixels;
    mp_pixels = nullptr;

    int width, height, channels;
    unsigned char *pixel_data = stbi_load(filename.c_str(), &width, &height, &channels, m_channels);

    if (pixel_data == NULL) throw std::runtime_error("Couldn\'t load texture");

    m_size.x      = static_cast<U32>(width);
    m_size.y      = static_cast<U32>(height);

    m_pixel_count = m_size.x * m_size.y;

    mp_pixels     = new Pixel[m_pixel_count];

    #pragma omp parallel for
    for (I32 p = 0u; p < m_pixel_count; ++p) {
        U32 pixel_data_start = p * m_channels;

        Pixel &pixel         = mp_pixels[p];
        pixel.color.r        = pixel_data[pixel_data_start];
        pixel.color.g        = pixel_data[pixel_data_start + 1];
        pixel.color.b        = pixel_data[pixel_data_start + 2];

        if (channels == 3) {
            pixel.color.a = 255u;
        } else if (channels == 4) {
            pixel.color.a = pixel_data[pixel_data_start + 3];
        }

        pixel.character      = character;
    }

    stbi_image_free(pixel_data);
}

void clgl::Texture::load_with_custom_characters(const std::string &filename, const std::string &characters) {
    delete[] mp_pixels;
    mp_pixels = nullptr;

    int width, height, channels;
    unsigned char *pixel_data = stbi_load(filename.c_str(), &width, &height, &channels, m_channels);

    if (pixel_data == NULL) throw std::runtime_error("Couldn\'t load texture");

    m_size.x      = static_cast<U32>(width);
    m_size.y      = static_cast<U32>(height);

    m_pixel_count = m_size.x * m_size.y;

    mp_pixels     = new Pixel[m_pixel_count];

    if (characters.size() != m_pixel_count) throw clgl::exceptions::InvalidParameter();

    #pragma omp parallel for
    for (I32 p = 0u; p < m_pixel_count; ++p) {
        U32 pixel_data_start = p * m_channels;

        Pixel &pixel         = mp_pixels[p];
        pixel.color.r        = pixel_data[pixel_data_start];
        pixel.color.g        = pixel_data[pixel_data_start + 1];
        pixel.color.b        = pixel_data[pixel_data_start + 2];

        if (channels == 3) {
            pixel.color.a = 255u;
        } else if (channels == 4) {
            pixel.color.a = pixel_data[pixel_data_start + 3];
        }

        pixel.character      = characters[p];
    }

    stbi_image_free(pixel_data);
}

const clgl::Vec2U &clgl::Texture::get_size() const {
    return m_size;
}

clgl::U32 clgl::Texture::get_pixel_count() const {
    return m_pixel_count;
}

clgl::U32 clgl::Texture::get_texture_index(const Vec2U &coordinates) const {
    return coordinates.x + coordinates.y * m_size.x;
}

const clgl::Pixel &clgl::Texture::get_pixel(U32 index) const {
    return mp_pixels[index];
}

const clgl::Pixel &clgl::Texture::get_pixel(const Vec2U &coordinates) const {
    return get_pixel(get_texture_index(coordinates));
}

const clgl::Pixel *clgl::Texture::get_pixel_data() const {
    return mp_pixels;
}

std::shared_ptr<clgl::Texture> clgl::Texture::create(const std::string &filename, Character character) {
    std::shared_ptr<clgl::Texture> texture_pointer = std::make_shared<Texture>();
    texture_pointer->load(filename, character);
    return texture_pointer;
}

std::shared_ptr<clgl::Texture> clgl::Texture::create_with_custom_characters(const std::string &filename, const std::string &characters) {
    std::shared_ptr<clgl::Texture> texture_pointer = std::make_shared<Texture>();
    texture_pointer->load_with_custom_characters(filename, characters);
    return texture_pointer;
}

void clgl::interpolators::nearest_neighbor(const Vec2F &coordinates, const Texture &texture, Pixel &output) {
    output = texture.get_pixel(coordinates);
}

void clgl::interpolators::bilinear(const Vec2F &coordinates, const Texture &texture, Pixel &output) {
    output.character = texture.get_pixel(coordinates).character;

    F32 x_floor = std::floor(coordinates.x); F32 y_floor = std::floor(coordinates.y);
    F32 fx = coordinates.x - x_floor; F32 fy = coordinates.y - y_floor;

    BilinearPixelData pixel_data[4];

    I32 texture_horizontal_start;
    I32 texture_horizontal_end;
    I32 texture_vertical_start;
    I32 texture_vertical_end;

    I32 data_horizontal_start    = 0;
    I32 data_horizontal_end      = 2;
    I32 data_vertical_start      = 0;
    I32 data_vertical_end        = 2;

    if (fx < 0.5f) texture_horizontal_start = static_cast<I32>(x_floor) - 1;
    else           texture_horizontal_start = static_cast<I32>(x_floor);
    texture_horizontal_end   = texture_horizontal_start + 2;

    if (fy < 0.5f) texture_vertical_start = static_cast<I32>(y_floor) - 1;
    else           texture_vertical_start = static_cast<I32>(y_floor);
    texture_vertical_end   = texture_vertical_start + 2;

    if (texture_horizontal_start < 0) {
        texture_horizontal_start = 0;
        data_horizontal_start    = 1;
    }

    if (texture_horizontal_end > texture.get_size().x) {
        texture_horizontal_end = texture.get_size().x;
        data_horizontal_end    = 1;
    }

    if (texture_vertical_start < 0) {
        texture_vertical_start = 0;
        data_vertical_start    = 1;
    }

    if (texture_vertical_end > texture.get_size().y) {
        texture_vertical_end = texture.get_size().y;
        data_vertical_end    = 1;
    }

    I32 texture_x = texture_horizontal_start;
    I32 texture_y = texture_vertical_start;

    F32 alpha_weights_sum = 0.f;

    for (I32 y = data_vertical_start; y < data_vertical_end; ++y) {
        for (I32 x = data_horizontal_start; x < data_horizontal_end; ++x) {
            I32 data_index = x + y * 2;
            I32 texture_index = texture_x + texture_y * texture.get_size().x;

            BilinearPixelData &pixel = pixel_data[data_index];

            pixel.color = texture.get_pixel(texture_index).color;
            pixel.f_alpha = pixel.color.a * _inverse_255;
            pixel.position = { static_cast<F32>(texture_x) + 0.5f, static_cast<F32>(texture_y) + 0.5f };

            Vec2F pixel_position = pixel.position;

            Vec2F distance_to_pixel = { std::abs(pixel_position.x - coordinates.x), std::abs(pixel_position.y - coordinates.y) };

            pixel.weight = (1.f - distance_to_pixel.x) * (1.f - distance_to_pixel.y);

            alpha_weights_sum += pixel.f_alpha * pixel.weight;

            ++texture_x;
        }
        texture_x = texture_horizontal_start;
        ++texture_y;
    }

    if (alpha_weights_sum == 0.f) {
        output.color.a = 0u;
        return;
    }

    F32 inverse_alpha_weights_sum = 1.f / alpha_weights_sum;

    output.color = {
        static_cast<U8>(
            (pixel_data[0].color.r * pixel_data[0].f_alpha * pixel_data[0].weight +
             pixel_data[1].color.r * pixel_data[1].f_alpha * pixel_data[1].weight +
             pixel_data[2].color.r * pixel_data[2].f_alpha * pixel_data[2].weight +
             pixel_data[3].color.r * pixel_data[3].f_alpha * pixel_data[3].weight) * inverse_alpha_weights_sum
        ),

        static_cast<U8>(
            (pixel_data[0].color.g * pixel_data[0].f_alpha * pixel_data[0].weight +
             pixel_data[1].color.g * pixel_data[1].f_alpha * pixel_data[1].weight +
             pixel_data[2].color.g * pixel_data[2].f_alpha * pixel_data[2].weight +
             pixel_data[3].color.g * pixel_data[3].f_alpha * pixel_data[3].weight) * inverse_alpha_weights_sum
        ),

        static_cast<U8>(
            (pixel_data[0].color.b * pixel_data[0].f_alpha * pixel_data[0].weight +
             pixel_data[1].color.b * pixel_data[1].f_alpha * pixel_data[1].weight +
             pixel_data[2].color.b * pixel_data[2].f_alpha * pixel_data[2].weight +
             pixel_data[3].color.b * pixel_data[3].f_alpha * pixel_data[3].weight) * inverse_alpha_weights_sum
        ),

        static_cast<U8>(
            pixel_data[0].color.a * pixel_data[0].weight +
            pixel_data[1].color.a * pixel_data[1].weight +
            pixel_data[2].color.a * pixel_data[2].weight +
            pixel_data[3].color.a * pixel_data[3].weight
        ),
    };
}
