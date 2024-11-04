#include "clgl/drawables/sprite.hpp"
#include <numbers>
#include "clgl/utils/utils.hpp"
#include <cmath>
#include <algorithm>
#include "clgl/drawables/intline.hpp"

void clgl::Sprite::draw(ScreenBuffer &screen_buffer) {
    if (m_rotation == 0.f) {
        draw_normal(screen_buffer);
    } else {
        draw_rotated(screen_buffer);
    }
}

void clgl::Sprite::draw_no_clipping(ScreenBuffer &screen_buffer) {
    if (m_rotation == 0.f) {
        draw_no_clipping_normal(screen_buffer);
    } else {
        draw_no_clipping_rotated(screen_buffer);
    }
}

void clgl::Sprite::set_position(const Vec2F &position) {
    m_topleft = position;
}

void clgl::Sprite::move(const Vec2F &offset) {
    m_topleft += offset;
}

void clgl::Sprite::set_rotation(F32 rotation) {
    m_rotation = rotation;
}

clgl::F32 clgl::Sprite::rotate(F32 angle) {
    m_rotation += angle;
    return m_rotation;
}

void clgl::Sprite::set_texture(std::shared_ptr<Texture> texture) {
    mp_texture = texture;
}

void clgl::Sprite::draw_normal(ScreenBuffer &screen_buffer) {
    Vec2I topleft = Vec2I(m_topleft);
    Vec2I bottomright = topleft + mp_texture->get_size();

    Vec2I clipped_topleft = {
        std::clamp(topleft.x, 0, static_cast<I32>(screen_buffer.get_size().x)),
        std::clamp(topleft.y, 0, static_cast<I32>(screen_buffer.get_size().y))
    };

    Vec2I clipped_bottomright = {
        std::clamp(bottomright.x, 0, static_cast<I32>(screen_buffer.get_size().x)),
        std::clamp(bottomright.y, 0, static_cast<I32>(screen_buffer.get_size().y))
    };

    #pragma omp parallel for
    for (I32 y = clipped_topleft.y; y < clipped_bottomright.y; ++y) {
        for (I32 x = clipped_topleft.x; x < clipped_bottomright.x; ++x) {
            screen_buffer.set_pixel(Vec2U(x, y), mp_texture->get_pixel({ static_cast<U32>(x - topleft.x), static_cast<U32>(y - topleft.y) }));
        }
    }
}

void clgl::Sprite::draw_rotated(ScreenBuffer &screen_buffer) {
    F32 angle_in_radians = m_rotation * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos        = std::cos(-angle_in_radians);
    F32 angle_sin        = std::sin(-angle_in_radians);

    I32 new_width        = std::ceil(
        std::abs(static_cast<F32>(mp_texture->get_size().x) * angle_cos) +
        std::abs(static_cast<F32>(mp_texture->get_size().y) * angle_sin)
    );
    I32 new_height       = std::ceil(
        std::abs(static_cast<F32>(mp_texture->get_size().x) * angle_sin) +
        std::abs(static_cast<F32>(mp_texture->get_size().y) * angle_cos)
    );

    Vec2F half_old_size { static_cast<F32>(mp_texture->get_size().x) * 0.5f, static_cast<F32>(mp_texture->get_size().y) * 0.5f };
    Vec2F half_new_size { static_cast<F32>(new_width) * 0.5f, static_cast<F32>(new_height) * 0.5f };
    Vec2F offset        = half_new_size - half_old_size;

    #pragma omp parallel for
    for (I32 y = 0u; y < new_height; ++y) {
        for (I32 x = 0u; x < new_width; ++x) {
            Vec2F destination_coordinates = Vec2F(x, y) - half_new_size;

            Vec2F rotated {
                destination_coordinates.x * (angle_cos) - destination_coordinates.y * (angle_sin) + half_old_size.x,
                destination_coordinates.y * (angle_cos) + destination_coordinates.x * (angle_sin) + half_old_size.y
            };

            if (
                rotated.x < 0 ||
                rotated.y < 0 ||
                rotated.x >= static_cast<F32>(mp_texture->get_size().x) ||
                rotated.y >= static_cast<F32>(mp_texture->get_size().y)
            ) continue;

            Color topleft_color, topright_color, bottomleft_color, bottomright_color;

            Vec2F topleft_pixel_topleft     { std::floor(rotated.x), std::floor(rotated.y) };

            Vec2I topleft_pixel_coordinates = topleft_pixel_topleft;
            I32 topleft_pixel_index         = topleft_pixel_coordinates.x + topleft_pixel_coordinates.y * mp_texture->get_size().x;

            Character character             = mp_texture->get_pixel(topleft_pixel_index).character;

            topleft_color                   = mp_texture->get_pixel(topleft_pixel_index).color;

            bool right_in                   = topleft_pixel_topleft.x + 1.f < static_cast<F32>(mp_texture->get_size().x);
            bool bottom_in                  = topleft_pixel_topleft.y + 1.f < static_cast<F32>(mp_texture->get_size().y);

            if (right_in && bottom_in) {
                topright_color    = mp_texture->get_pixel(topleft_pixel_index + 1).color;
                bottomleft_color  = mp_texture->get_pixel(topleft_pixel_index + mp_texture->get_size().x).color;
                bottomright_color = mp_texture->get_pixel(topleft_pixel_index + mp_texture->get_size().x + 1).color;
            } else if (right_in) {
                topright_color    = mp_texture->get_pixel(topleft_pixel_index + 1).color;
                bottomleft_color  = topleft_color;
                bottomright_color = Color(
                    static_cast<U8>((static_cast<U32>(topleft_color.r) + static_cast<U32>(topright_color.r)) >> 2),
                    static_cast<U8>((static_cast<U32>(topleft_color.g) + static_cast<U32>(topright_color.g)) >> 2),
                    static_cast<U8>((static_cast<U32>(topleft_color.b) + static_cast<U32>(topright_color.b)) >> 2)
                );
            } else if (bottom_in) {
                topright_color    = topleft_color;
                bottomleft_color  = mp_texture->get_pixel(topleft_pixel_index + mp_texture->get_size().x).color;
                bottomright_color = Color(
                    static_cast<U8>((static_cast<U32>(topleft_color.r) + static_cast<U32>(bottomleft_color.r)) >> 2),
                    static_cast<U8>((static_cast<U32>(topleft_color.g) + static_cast<U32>(bottomleft_color.g)) >> 2),
                    static_cast<U8>((static_cast<U32>(topleft_color.b) + static_cast<U32>(bottomleft_color.b)) >> 2)
                );
            } else {
                topright_color    = topleft_color;
                bottomleft_color  = topleft_color;
                bottomright_color = topleft_color;
            }

            F32 fx = rotated.x - topleft_pixel_topleft.x;
            F32 fy = rotated.y - topleft_pixel_topleft.y;

            F32 topleft_weight     = (1.f - fx) * (1.f - fy);
            F32 topright_weight    = fx * (1.f - fy);
            F32 bottomleft_weight  = (1.f - fx) * fy;
            F32 bottomright_weight = fx * fy;

            Color interpolated_color {
                static_cast<U8>(
                    topleft_color.r     * topleft_weight    +
                    topright_color.r    * topright_weight   +
                    bottomleft_color.r  * bottomleft_weight +
                    bottomright_color.r * bottomright_weight
                ),
                static_cast<U8>(
                    topleft_color.g     * topleft_weight    +
                    topright_color.g    * topright_weight   +
                    bottomleft_color.g  * bottomleft_weight +
                    bottomright_color.g * bottomright_weight
                ),
                static_cast<U8>(
                    topleft_color.b     * topleft_weight    +
                    topright_color.b    * topright_weight   +
                    bottomleft_color.b  * bottomleft_weight +
                    bottomright_color.b * bottomright_weight
                ),
            };

            screen_buffer.set_pixel_safe(Vec2F(x, y) - offset + m_topleft, Pixel(interpolated_color, character));
        }
    }
}

void clgl::Sprite::draw_no_clipping_normal(ScreenBuffer &screen_buffer) {
    Vec2U topleft = Vec2U(m_topleft);
    #pragma omp parallel for
    for (I32 y = 0u; y < mp_texture->get_size().y; ++y) {
        for (I32 x = 0u; x < mp_texture->get_size().x; ++x) {
            screen_buffer.set_pixel(topleft + Vec2U(x, y), mp_texture->get_pixel({ static_cast<U32>(x), static_cast<U32>(y) }));
        }
    }
}

void clgl::Sprite::draw_no_clipping_rotated(ScreenBuffer &screen_buffer) {
    F32 angle_in_radians = m_rotation * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos        = std::cos(-angle_in_radians);
    F32 angle_sin        = std::sin(-angle_in_radians);

    I32 new_width        = std::ceil(
        std::abs(static_cast<F32>(mp_texture->get_size().x) * angle_cos) +
        std::abs(static_cast<F32>(mp_texture->get_size().y) * angle_sin)
    );
    I32 new_height       = std::ceil(
        std::abs(static_cast<F32>(mp_texture->get_size().x) * angle_sin) +
        std::abs(static_cast<F32>(mp_texture->get_size().y) * angle_cos)
    );

    Vec2F half_old_size { static_cast<F32>(mp_texture->get_size().x) * 0.5f, static_cast<F32>(mp_texture->get_size().y) * 0.5f };
    Vec2F half_new_size { static_cast<F32>(new_width) * 0.5f, static_cast<F32>(new_height) * 0.5f };
    Vec2F offset        = half_new_size - half_old_size;

    #pragma omp parallel for
    for (I32 y = 0u; y < new_height; ++y) {
        for (I32 x = 0u; x < new_width; ++x) {
            Vec2F destination_coordinates = Vec2F(x, y) - half_new_size;

            Vec2F rotated {
                destination_coordinates.x * (angle_cos) - destination_coordinates.y * (angle_sin) + half_old_size.x,
                destination_coordinates.y * (angle_cos) + destination_coordinates.x * (angle_sin) + half_old_size.y
            };

            if (
                rotated.x < 0 ||
                rotated.y < 0 ||
                rotated.x >= static_cast<F32>(mp_texture->get_size().x) ||
                rotated.y >= static_cast<F32>(mp_texture->get_size().y)
            ) continue;

            Color topleft_color, topright_color, bottomleft_color, bottomright_color;

            Vec2F topleft_pixel_topleft     { std::floor(rotated.x), std::floor(rotated.y) };

            Vec2I topleft_pixel_coordinates = topleft_pixel_topleft;
            I32 topleft_pixel_index         = topleft_pixel_coordinates.x + topleft_pixel_coordinates.y * mp_texture->get_size().x;

            Character character             = mp_texture->get_pixel(topleft_pixel_index).character;

            topleft_color                   = mp_texture->get_pixel(topleft_pixel_index).color;

            bool right_in                   = topleft_pixel_topleft.x + 1.f < static_cast<F32>(mp_texture->get_size().x);
            bool bottom_in                  = topleft_pixel_topleft.y + 1.f < static_cast<F32>(mp_texture->get_size().y);

            if (right_in && bottom_in) {
                topright_color    = mp_texture->get_pixel(topleft_pixel_index + 1).color;
                bottomleft_color  = mp_texture->get_pixel(topleft_pixel_index + mp_texture->get_size().x).color;
                bottomright_color = mp_texture->get_pixel(topleft_pixel_index + mp_texture->get_size().x + 1).color;
            } else if (right_in) {
                topright_color    = mp_texture->get_pixel(topleft_pixel_index + 1).color;
                bottomleft_color  = topleft_color;
                bottomright_color = Color(
                    static_cast<U8>((static_cast<U32>(topleft_color.r) + static_cast<U32>(topright_color.r)) >> 2),
                    static_cast<U8>((static_cast<U32>(topleft_color.g) + static_cast<U32>(topright_color.g)) >> 2),
                    static_cast<U8>((static_cast<U32>(topleft_color.b) + static_cast<U32>(topright_color.b)) >> 2)
                );
            } else if (bottom_in) {
                topright_color    = topleft_color;
                bottomleft_color  = mp_texture->get_pixel(topleft_pixel_index + mp_texture->get_size().x).color;
                bottomright_color = Color(
                    static_cast<U8>((static_cast<U32>(topleft_color.r) + static_cast<U32>(bottomleft_color.r)) >> 2),
                    static_cast<U8>((static_cast<U32>(topleft_color.g) + static_cast<U32>(bottomleft_color.g)) >> 2),
                    static_cast<U8>((static_cast<U32>(topleft_color.b) + static_cast<U32>(bottomleft_color.b)) >> 2)
                );
            } else {
                topright_color    = topleft_color;
                bottomleft_color  = topleft_color;
                bottomright_color = topleft_color;
            }

            F32 fx = rotated.x - topleft_pixel_topleft.x;
            F32 fy = rotated.y - topleft_pixel_topleft.y;

            F32 topleft_weight     = (1.f - fx) * (1.f - fy);
            F32 topright_weight    = fx * (1.f - fy);
            F32 bottomleft_weight  = (1.f - fx) * fy;
            F32 bottomright_weight = fx * fy;

            Color interpolated_color {
                static_cast<U8>(
                    topleft_color.r     * topleft_weight    +
                    topright_color.r    * topright_weight   +
                    bottomleft_color.r  * bottomleft_weight +
                    bottomright_color.r * bottomright_weight
                ),
                static_cast<U8>(
                    topleft_color.g     * topleft_weight    +
                    topright_color.g    * topright_weight   +
                    bottomleft_color.g  * bottomleft_weight +
                    bottomright_color.g * bottomright_weight
                ),
                static_cast<U8>(
                    topleft_color.b     * topleft_weight    +
                    topright_color.b    * topright_weight   +
                    bottomleft_color.b  * bottomleft_weight +
                    bottomright_color.b * bottomright_weight
                ),
            };

            screen_buffer.set_pixel(Vec2F(x, y) - offset + m_topleft, Pixel(interpolated_color, character));
        }
    }
}