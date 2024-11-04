#include "clgl/texture.hpp"
#include "clgl/../external/stb_image.h"
#include <stdexcept>
#include "clgl/exceptions.hpp"

clgl::Texture::~Texture() {
    delete[] mp_pixels;
}

void clgl::Texture::load(const std::string &filename, Character character) {
    delete[] mp_pixels;
    mp_pixels = nullptr;

    int width, height, channels;
    unsigned char *pixel_data = stbi_load(filename.c_str(), &width, &height, &channels, 3);

    if (pixel_data == NULL) throw std::runtime_error("Couldn\'t load texture");

    m_size.x      = static_cast<U32>(width);
    m_size.y      = static_cast<U32>(height);

    m_pixel_count = m_size.x * m_size.y;

    mp_pixels     = new Pixel[m_pixel_count];

    #pragma omp parallel for
    for (I32 p = 0u; p < m_pixel_count; ++p) {
        U32 pixel_data_start = p * 3;

        Pixel &pixel         = mp_pixels[p];
        pixel.color.r        = pixel_data[pixel_data_start];
        pixel.color.g        = pixel_data[pixel_data_start + 1];
        pixel.color.b        = pixel_data[pixel_data_start + 2];

        pixel.character      = character;
    }

    stbi_image_free(pixel_data);
}

void clgl::Texture::load_with_custom_characters(const std::string &filename, const std::string &characters) {
    delete[] mp_pixels;
    mp_pixels = nullptr;

    int width, height, channels;
    unsigned char *pixel_data = stbi_load(filename.c_str(), &width, &height, &channels, 3);

    if (pixel_data == NULL) throw std::runtime_error("Couldn\'t load texture");

    m_size.x      = static_cast<U32>(width);
    m_size.y      = static_cast<U32>(height);

    m_pixel_count = m_size.x * m_size.y;

    mp_pixels     = new Pixel[m_pixel_count];

    if (characters.size() != m_pixel_count) throw clgl::exceptions::InvalidParameter();

    #pragma omp parallel for
    for (I32 p = 0u; p < m_pixel_count; ++p) {
        U32 pixel_data_start = p * 3;

        Pixel &pixel         = mp_pixels[p];
        pixel.color.r        = pixel_data[pixel_data_start];
        pixel.color.g        = pixel_data[pixel_data_start + 1];
        pixel.color.b        = pixel_data[pixel_data_start + 2];

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