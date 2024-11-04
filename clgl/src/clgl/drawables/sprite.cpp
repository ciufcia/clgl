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
        std::clamp(topleft.x, 0, static_cast<I32>(screen_buffer.get_max_int_coordinates().x)),
        std::clamp(topleft.y, 0, static_cast<I32>(screen_buffer.get_max_int_coordinates().y))
    };

    Vec2I clipped_bottomright = {
        std::clamp(bottomright.x, 0, static_cast<I32>(screen_buffer.get_max_int_coordinates().x)),
        std::clamp(bottomright.y, 0, static_cast<I32>(screen_buffer.get_max_int_coordinates().y))
    };

    Vec2I topleft_shift = clipped_topleft - topleft;
    I32 shifted_width = mp_texture->get_size().x - topleft_shift.x;
    I32 clipped_texture_topleft = topleft_shift.x + topleft_shift.y * mp_texture->get_size().x;
    I32 texture_index = clipped_texture_topleft;
    I32 y_counter = 0u;

    U32 current_index = 0u;
    for (U32 y = 0u; y < mp_texture->get_size().y; ++y) {
        current_index = y * mp_texture->get_size().x;
        for (U32 x = 0u; x < mp_texture->get_size().x; ++x) {
            screen_buffer.set_pixel(topleft + Vec2U(x, y), mp_texture->get_pixel_data()[current_index]);
            ++current_index;
        }
    }
}

void clgl::Sprite::draw_rotated(ScreenBuffer &screen_buffer) {
    F32 angle_in_radians = m_rotation * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos = std::cos(-angle_in_radians);
    F32 angle_sin = std::sin(-angle_in_radians);

    I32 new_width = std::ceil(
        std::abs(static_cast<F32>(mp_texture->get_size().x) * angle_cos) +
        std::abs(static_cast<F32>(mp_texture->get_size().y) * angle_sin)
    );
    I32 new_height = std::ceil(
        std::abs(static_cast<F32>(mp_texture->get_size().x) * angle_sin) +
        std::abs(static_cast<F32>(mp_texture->get_size().y) * angle_cos)
    );

    Vec2F half_old_size { static_cast<F32>(mp_texture->get_size().x) * 0.5f, static_cast<F32>(mp_texture->get_size().y) * 0.5f };
    Vec2F half_new_size { static_cast<F32>(new_width) * 0.5f, static_cast<F32>(new_height) * 0.5f };

    Vec2F offset = half_new_size - half_old_size;

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

            Vec2F topleft_pixel_topleft = { std::floor(rotated.x), std::floor(rotated.y) };

            Vec2I topleft_pixel_coordinates = topleft_pixel_topleft;
            I32 topleft_pixel_index = topleft_pixel_coordinates.x + topleft_pixel_coordinates.y * mp_texture->get_size().x;

            Character character = mp_texture->get_pixel_data()[topleft_pixel_index].character;

            Color topleft_color, topright_color, bottomleft_color, bottomright_color;

            topleft_color = mp_texture->get_pixel_data()[topleft_pixel_index].color;

            bool right_in = topleft_pixel_topleft.x + 1.f < static_cast<F32>(mp_texture->get_size().x);
            bool bottom_in = topleft_pixel_topleft.y + 1.f < static_cast<F32>(mp_texture->get_size().y);

            if (right_in && bottom_in) {
                topright_color = mp_texture->get_pixel_data()[topleft_pixel_index + 1].color;
                bottomleft_color = mp_texture->get_pixel_data()[topleft_pixel_index + mp_texture->get_size().x].color;
                bottomright_color = mp_texture->get_pixel_data()[topleft_pixel_index + mp_texture->get_size().x + 1].color;
            } else if (right_in) {
                topright_color = mp_texture->get_pixel_data()[topleft_pixel_index + 1].color;
                bottomleft_color = topleft_color;
                bottomright_color = Color(
                    (topleft_color.r + topright_color.r) >> 2,
                    (topleft_color.g + topright_color.g) >> 2,
                    (topleft_color.b + topright_color.b) >> 2
                );
            } else if (bottom_in) {
                topright_color = topleft_color;
                bottomleft_color = mp_texture->get_pixel_data()[topleft_pixel_index + mp_texture->get_size().x].color;
                bottomright_color = Color(
                    (topleft_color.r + bottomleft_color.r) >> 2,
                    (topleft_color.g + bottomleft_color.g) >> 2,
                    (topleft_color.b + bottomleft_color.b) >> 2
                );
            } else {
                topright_color = topleft_color;
                bottomleft_color = topleft_color;
                bottomright_color = topleft_color;
            }

            F32 fx = rotated.x - topleft_pixel_topleft.x;
            F32 fy = rotated.y - topleft_pixel_topleft.y;

            F32 topleft_weight = (1.f - fx) * (1.f - fy);
            F32 topright_weight = fx * (1.f - fy);
            F32 bottomleft_weight = (1.f - fx) * fy;
            F32 bottomright_weight = fx * fy;

            Color interpolated_color {
                static_cast<U8>(
                    topleft_color.r * topleft_weight +
                    topright_color.r * topright_weight +
                    bottomleft_color.r * bottomleft_weight +
                    bottomright_color.r * bottomright_weight
                ),
                static_cast<U8>(
                    topleft_color.g * topleft_weight +
                    topright_color.g * topright_weight +
                    bottomleft_color.g * bottomleft_weight +
                    bottomright_color.g * bottomright_weight
                ),
                static_cast<U8>(
                    topleft_color.b * topleft_weight +
                    topright_color.b * topright_weight +
                    bottomleft_color.b * bottomleft_weight +
                    bottomright_color.b * bottomright_weight
                ),
            };

            screen_buffer.set_pixel_safe(Vec2F(x, y) - offset + m_topleft, Pixel(interpolated_color, character));
        }
    }
}

void clgl::Sprite::draw_no_clipping_normal(ScreenBuffer &screen_buffer) {
    Vec2U topleft = Vec2U(m_topleft);
    U32 current_index = 0u;
    for (U32 y = 0u; y < mp_texture->get_size().y; ++y) {
        current_index = y * mp_texture->get_size().x;
        for (U32 x = 0u; x < mp_texture->get_size().x; ++x) {
            screen_buffer.set_pixel(topleft + Vec2U(x, y), mp_texture->get_pixel_data()[current_index]);
            ++current_index;
        }
    }
}

void clgl::Sprite::draw_no_clipping_rotated(ScreenBuffer &screen_buffer) {
    F32 angle_in_radians = m_rotation * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos = std::cos(-angle_in_radians);
    F32 angle_sin = std::sin(-angle_in_radians);

    I32 new_width = std::ceil(
        std::abs(static_cast<F32>(mp_texture->get_size().x) * angle_cos) + 
        std::abs(static_cast<F32>(mp_texture->get_size().y) * angle_sin)
    );
    I32 new_height = std::ceil(
        std::abs(static_cast<F32>(mp_texture->get_size().x) * angle_sin) +
        std::abs(static_cast<F32>(mp_texture->get_size().y) * angle_cos)
    );

    Vec2F half_old_size { static_cast<F32>(mp_texture->get_size().x) * 0.5f, static_cast<F32>(mp_texture->get_size().y) * 0.5f };
    Vec2F half_new_size { static_cast<F32>(new_width) * 0.5f, static_cast<F32>(new_height) * 0.5f };

    Vec2F offset = half_new_size - half_old_size;

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

            Vec2F topleft_pixel_topleft = { std::floor(rotated.x), std::floor(rotated.y) };

            Vec2I topleft_pixel_coordinates = topleft_pixel_topleft;
            I32 topleft_pixel_index = topleft_pixel_coordinates.x + topleft_pixel_coordinates.y * mp_texture->get_size().x;

            Character character = mp_texture->get_pixel_data()[topleft_pixel_index].character;

            Color topleft_color, topright_color, bottomleft_color, bottomright_color;

            topleft_color = mp_texture->get_pixel_data()[topleft_pixel_index].color;

            bool right_in = topleft_pixel_topleft.x + 1.f < static_cast<F32>(mp_texture->get_size().x);
            bool bottom_in = topleft_pixel_topleft.y + 1.f < static_cast<F32>(mp_texture->get_size().y);

            if (right_in && bottom_in) {
                topright_color = mp_texture->get_pixel_data()[topleft_pixel_index + 1].color;
                bottomleft_color = mp_texture->get_pixel_data()[topleft_pixel_index + mp_texture->get_size().x].color;
                bottomright_color = mp_texture->get_pixel_data()[topleft_pixel_index + mp_texture->get_size().x + 1].color;
            } else if (right_in) {
                topright_color = mp_texture->get_pixel_data()[topleft_pixel_index + 1].color;
                bottomleft_color = topleft_color;
                bottomright_color = topleft_color;
            } else if (bottom_in) {
                topright_color = topleft_color;
                bottomleft_color = mp_texture->get_pixel_data()[topleft_pixel_index + mp_texture->get_size().x].color;
                bottomright_color = topleft_color;
            } else {
                topright_color = topleft_color;
                bottomleft_color = topleft_color;
                bottomright_color = topleft_color;
            }

            F32 fx = rotated.x - topleft_pixel_topleft.x;
            F32 fy = rotated.y - topleft_pixel_topleft.y;

            F32 topleft_weight = (1.f - fx) * (1.f - fy);
            F32 topright_weight = fx * (1.f - fy);
            F32 bottomleft_weight = (1.f - fx) * fy;
            F32 bottomright_weight = fx * fy;

            Color interpolated_color {
                static_cast<U8>(
                    topleft_color.r * topleft_weight +
                    topright_color.r * topright_weight +
                    bottomleft_color.r * bottomleft_weight +
                    bottomright_color.r * bottomright_weight
                ),
                static_cast<U8>(
                    topleft_color.g * topleft_weight +
                    topright_color.g * topright_weight +
                    bottomleft_color.g * bottomleft_weight +
                    bottomright_color.g * bottomright_weight
                ),
                static_cast<U8>(
                    topleft_color.b * topleft_weight +
                    topright_color.b * topright_weight +
                    bottomleft_color.b * bottomleft_weight +
                    bottomright_color.b * bottomright_weight
                ),
            };

            screen_buffer.set_pixel(Vec2F(x, y) - offset + m_topleft, Pixel(interpolated_color, character));
        }
    }
}

void clgl::utils::x_shear(F32 shear, Vec2F *target, U32 width, U32 height) {
    U32 texture_index = 0u;
    for (U32 y = 0u; y < height; ++y) {
        texture_index = y * width;
        for (U32 x = 0u; x < width; ++x) {
            target[texture_index] += shear * y;

            ++texture_index;
        }
    }
}