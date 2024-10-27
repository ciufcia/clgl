#include "clgl/screen_buffer.hpp"
#include <cmath>

clgl::ScreenBuffer::ScreenBuffer(const Vec2U &size) {
    set_size(size);
}

clgl::ScreenBuffer::~ScreenBuffer() {
    delete[] mp_pixels;
}

void clgl::ScreenBuffer::set_pixel(U32 index, const Pixel &pixel) {
    mp_pixels[index] = pixel;
}

void clgl::ScreenBuffer::set_pixel_safe(U32 index, const Pixel &pixel) {
    if (index < 0u || index >= m_pixel_count) return;
    set_pixel(index, pixel);
}

void clgl::ScreenBuffer::set_pixel(const Vec2U &coords, const Pixel &pixel) {
    const U32 index = get_buffer_index(coords);
    set_pixel(index, pixel);
}

void clgl::ScreenBuffer::set_pixel_safe(const Vec2U &coords, const Pixel &pixel) {
    if (coords.x < 0u || coords.x >= m_size.x || coords.y < 0u || coords.y >= m_size.y) return;
    set_pixel(coords, pixel);
}

clgl::Pixel &clgl::ScreenBuffer::get_pixel(U32 index) {
    return mp_pixels[index];
}

clgl::Pixel &clgl::ScreenBuffer::get_pixel(const Vec2U &coords) {
    const U32 index = get_buffer_index(coords);
    return mp_pixels[index];
}

const clgl::Pixel &clgl::ScreenBuffer::get_pixel(U32 index) const {
    return mp_pixels[index];
}

const clgl::Pixel &clgl::ScreenBuffer::get_pixel(const Vec2U &coords) const {
    const U32 index = get_buffer_index(coords);
    return mp_pixels[index];
}

clgl::U32 clgl::ScreenBuffer::get_buffer_index(const Vec2U &coords) const {
    return coords.x + coords.y * m_size.x;
}

const clgl::Vec2U &clgl::ScreenBuffer::get_size() const {
    return m_size;
}

void clgl::ScreenBuffer::set_size(const Vec2U &size) {
    m_size = size;

    m_max_float_coordinates = { size.x - get_max_float_coordinate_offset(), size.y - get_max_float_coordinate_offset() };
    m_max_int_coordinates   = { size.x - 1u, size.y - 1u };

    m_pixel_count = size.x * size.y;

    delete[] mp_pixels;
    mp_pixels = new Pixel[m_pixel_count];
}

const clgl::Vec2F &clgl::ScreenBuffer::get_max_float_coordinates() const {
    return m_max_float_coordinates;
}

clgl::F32 clgl::ScreenBuffer::get_max_float_coordinate_offset() const {
    return m_max_float_coordinate_offset;
}

const clgl::Vec2U &clgl::ScreenBuffer::get_max_int_coordinates() const {
    return m_max_int_coordinates;
}

const clgl::Pixel *clgl::ScreenBuffer::get_pixels() const {
    return mp_pixels;
}

clgl::U32 clgl::ScreenBuffer::get_pixel_count() const {
    return m_pixel_count;
}