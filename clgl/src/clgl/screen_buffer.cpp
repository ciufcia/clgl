#include "clgl/screen_buffer.hpp"
#include "clgl/utils/utils.hpp"
#include <cmath>
#include "clgl/exceptions.hpp"

clgl::ScreenBuffer::ScreenBuffer(const Vec2U &size) {
    set_size(size);
}

clgl::ScreenBuffer::~ScreenBuffer() {
    delete[] mp_pixels;
}

void clgl::ScreenBuffer::set_pixel(U32 index, const Pixel &pixel) {
    if (!pixel.color.a) return;
    if (m_color_blending_enabled) {
        Color old_color = mp_pixels[index].color;
        mp_pixels[index].color = utils::blend_colors(pixel.color, old_color);
        mp_pixels[index].character = pixel.character;
    } else {
        mp_pixels[index] = pixel;
    }
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
    if (coords.x >= m_size.x || coords.y >= m_size.y) return;
    set_pixel(coords, pixel);
}

void clgl::ScreenBuffer::set_pixel_without_overdraw(U32 index, const Pixel &pixel) {
    if (!pixel.color.a) return;

    FlagBufferType flags = get_flags(index);
    if (_CLGL_BIT_MASK(flags, constants::overdraw_flag)) return;
    flags |= constants::overdraw_flag;
    set_flags(index, flags);

    if (m_color_blending_enabled) {
        Color old_color = mp_pixels[index].color;
        mp_pixels[index].color = utils::blend_colors(pixel.color, old_color);
        mp_pixels[index].character = pixel.character;
    } else {
        mp_pixels[index] = pixel;
    }
}

void clgl::ScreenBuffer::set_pixel_safe_without_overdraw(U32 index, const Pixel &pixel) {
    if (index < 0u || index >= m_pixel_count) return;
    set_pixel_without_overdraw(index, pixel);
}

void clgl::ScreenBuffer::set_pixel_without_overdraw(const Vec2U &coords, const Pixel &pixel) {
    const U32 index = get_buffer_index(coords);
    set_pixel_without_overdraw(index, pixel);
}

void clgl::ScreenBuffer::set_pixel_safe_without_overdraw(const Vec2U &coords, const Pixel &pixel) {
    if (coords.x >= m_size.x || coords.y >= m_size.y) return;
    set_pixel_without_overdraw(coords, pixel);
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
    if (size.x == 0u || size.y == 0u) throw exceptions::InvalidParameter();

    m_size = size;

    m_max_float_coordinates = { size.x - get_max_float_coordinate_offset(), size.y - get_max_float_coordinate_offset() };
    m_max_int_coordinates   = { size.x - 1u, size.y - 1u };

    m_pixel_count = size.x * size.y;

    delete[] mp_pixels;
    mp_pixels = new Pixel[m_pixel_count];

    delete[] mp_flag_buffer;
    mp_flag_buffer = new FlagBufferType[m_pixel_count] { 0u };

    m_flag_changes = {};
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

void clgl::ScreenBuffer::enable_color_blending(bool value) {
    m_color_blending_enabled = value;
}

bool clgl::ScreenBuffer::is_color_blending_enabled() const {
    return m_color_blending_enabled;
}

void clgl::ScreenBuffer::set_flags(U32 index, FlagBufferType flags) {
    m_flag_changes.push_back(index);
    mp_flag_buffer[index] = flags;
}

void clgl::ScreenBuffer::set_flags_safe(U32 index, FlagBufferType flags) {
    if (index < 0u || index >= m_pixel_count) return;
    set_flags(index, flags);
}

void clgl::ScreenBuffer::set_flags(const Vec2U &coords, FlagBufferType flags) {
    const U32 index = get_buffer_index(coords);
    set_flags(index, flags);
}

void clgl::ScreenBuffer::set_flags_safe(const Vec2U &coords, FlagBufferType flags) {
    if (coords.x >= m_size.x || coords.y >= m_size.y) return;
    set_flags(coords, flags);
}

clgl::FlagBufferType clgl::ScreenBuffer::get_flags(U32 index) const {
    return mp_flag_buffer[index];
}

clgl::FlagBufferType clgl::ScreenBuffer::get_flags(const Vec2U &coords) const {
    const U32 index = get_buffer_index(coords);
    return mp_flag_buffer[index];
}

void clgl::ScreenBuffer::clear_flags(FlagBufferType flags) {
    #pragma omp parallel for
    for (I32 i = 0; i < get_pixel_count(); ++i) {
        FlagBufferType old_flags = get_flags(i);
        FlagBufferType new_flags = _CLGL_BIT_MASK((~flags), old_flags);
        set_flags(i, new_flags);
    }

    for (I32 i = static_cast<I32>(m_flag_changes.size()) - 1; i > 0; --i) {
        U32 flag_index = m_flag_changes[i];
        FlagBufferType new_flags = get_flags(flag_index);

        if (new_flags == 0u) m_flag_changes.erase(m_flag_changes.begin() + i);
    }
}

void clgl::ScreenBuffer::clear_changed_flags(FlagBufferType flags) {
    for (I32 i = static_cast<I32>(m_flag_changes.size()) - 1; i >= 0; --i) {
        U32 flag_index = m_flag_changes[i];
        FlagBufferType old_flags = get_flags(flag_index);
        FlagBufferType new_flags = _CLGL_BIT_MASK(old_flags, (~flags));
        mp_flag_buffer[flag_index] = new_flags;

       if (new_flags == 0u) m_flag_changes.erase(m_flag_changes.begin() + i);
    }
}

const clgl::interpolators::PointerToInterpolator clgl::ScreenBuffer::get_current_interpolator() {
    return mp_interpolator;
}

void clgl::ScreenBuffer::set_interpolator(interpolators::PointerToInterpolator interpolator) {
    mp_interpolator = interpolator;
}