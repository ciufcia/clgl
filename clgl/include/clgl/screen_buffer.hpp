#pragma once

#include "pixel.hpp"
#include "vec.hpp"

namespace clgl
{
class ScreenBuffer
{
public:

                               ScreenBuffer() = default;
                               ScreenBuffer(const Vec2U &size);

                               ~ScreenBuffer();

    void                       set_pixel(U32 index, const Pixel &pixel);
    void                       set_pixel_safe(U32 index, const Pixel &pixel);
    void                       set_pixel(const Vec2U &coords, const Pixel &pixel);
    void                       set_pixel_safe(const Vec2U &coords, const Pixel &pixel);

    [[nodiscard]] Pixel &      get_pixel(U32 index);
    [[nodiscard]] Pixel &      get_pixel(const Vec2U &coords);
    [[nodiscard]] const Pixel &get_pixel(U32 index) const;
    [[nodiscard]] const Pixel &get_pixel(const Vec2U &coords) const;

    [[nodiscard]] U32          get_buffer_index(const Vec2U &coords) const;

    [[nodiscard]] const Vec2U &get_size() const;
    void                       set_size(const Vec2U &size);

    [[nodiscard]] const Vec2F &get_max_float_coordinates() const;
    [[nodiscard]] F32          get_max_float_coordinate_offset() const;

    [[nodiscard]] const Vec2U &get_max_int_coordinates() const;

    [[nodiscard]] const Pixel *get_pixels() const;
    [[nodiscard]] U32          get_pixel_count() const;

                  void         enable_color_blending(bool value = true);
    [[nodiscard]] bool         is_color_blending_enabled() const;

private:

    Vec2U     m_size                        { 0u, 0u };

    Vec2F     m_max_float_coordinates       { 0.f, 0.f };
    const F32 m_max_float_coordinate_offset = 0.00001f;

    Vec2U     m_max_int_coordinates         { 0, 0 };

    U32       m_pixel_count                 = 0u;

    Pixel *   mp_pixels                     = nullptr;

    bool      m_color_blending_enabled      = false;
};
}