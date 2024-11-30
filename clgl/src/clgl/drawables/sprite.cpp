#include "clgl/drawables/sprite.hpp"
#include <numbers>
#include "clgl/utils/utils.hpp"
#include <cmath>
#include <algorithm>
#include "clgl/drawables/intline.hpp"

void clgl::Sprite::draw(ScreenBuffer &screen_buffer) {
    if (get_rotation() != 0.f || get_scale() != Vec2F(1.f, 1.f)) {
        draw_transformed(screen_buffer);
    } else {
        draw_nontransformed(screen_buffer);
    }
}

void clgl::Sprite::draw_no_clipping(ScreenBuffer &screen_buffer) {
    if (get_rotation() != 0.f || get_scale() != Vec2F(1.f, 1.f)) {
        draw_no_clipping_transformed(screen_buffer);
    } else {
        draw_no_clipping_nontransformed(screen_buffer);
    }
}

void clgl::Sprite::set_texture(std::shared_ptr<Texture> texture) {
    mp_texture = texture;
}

void clgl::Sprite::draw_transformed(ScreenBuffer &screen_buffer) {
    F32 angle_in_radians = m_rotation * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos = std::cos(angle_in_radians);
    F32 angle_sin = std::sin(angle_in_radians);

    Vec2F transformed_corners[4] {
        { 0.f, 0.f },
        { static_cast<F32>(mp_texture->get_size().x), 0.f },
        { static_cast<F32>(mp_texture->get_size().x), static_cast<F32>(mp_texture->get_size().y) },
        { 0.f, static_cast<F32>(mp_texture->get_size().y) }
    };

    Vec2F topleft = { std::numeric_limits<F32>::max(), std::numeric_limits<F32>::max() };
    Vec2F bottomright = { std::numeric_limits<F32>::min(), std::numeric_limits<F32>::min() };

    for (I32 i = 0u; i < 4; ++i) {
        Vec2F &corner = transformed_corners[i];

        apply_scaling(corner);
        apply_rotation(corner, angle_cos, angle_sin);

        if (corner.x < topleft.x)     topleft.x = corner.x;
        if (corner.y < topleft.y)     topleft.y = corner.y;
        if (corner.x > bottomright.x) bottomright.x = corner.x;
        if (corner.y > bottomright.y) bottomright.y = corner.y;
    }

    Vec2I size = bottomright - topleft;

    //#pragma omp parallel for
    for (I32 y = 0; y <= size.y; ++y) {
        for (I32 x = 0; x <= size.x; ++x) {
            Vec2F destination_coordinates = Vec2F(x, y) + topleft;

            Vec2F translated = destination_coordinates;

            apply_inverse_rotation(translated, angle_cos, angle_sin);
            apply_inverse_scaling(translated);

            if (
                translated.x < 0.f ||
                translated.y < 0.f ||
                translated.x >= static_cast<F32>(mp_texture->get_size().x) ||
                translated.y >= static_cast<F32>(mp_texture->get_size().y)
                ) continue;

            Pixel pixel;
            screen_buffer.get_current_interpolator()(translated, *mp_texture, pixel);

            apply_translation(destination_coordinates);

            destination_coordinates = {
                std::floorf(destination_coordinates.x),
                std::floorf(destination_coordinates.y)
            };

            screen_buffer.set_pixel_safe(destination_coordinates, pixel);
        }
    }
}

void clgl::Sprite::draw_nontransformed(ScreenBuffer &screen_buffer) {
    Vec2I topleft = Vec2I(get_position() - get_origin());
    Vec2I bottomright = topleft + mp_texture->get_size();

    Vec2I texture_offset = topleft * -1.f;

    topleft.x = std::clamp(topleft.x, 0, static_cast<I32>(screen_buffer.get_max_int_coordinates().x));
    topleft.y = std::clamp(topleft.y, 0, static_cast<I32>(screen_buffer.get_max_int_coordinates().y));
    bottomright.x = std::clamp(bottomright.x, 0, static_cast<I32>(screen_buffer.get_max_int_coordinates().x));
    bottomright.y = std::clamp(bottomright.y, 0, static_cast<I32>(screen_buffer.get_max_int_coordinates().y));

    #pragma omp parallel for
    for (I32 y = topleft.y; y < bottomright.y; ++y) {
        for (I32 x = topleft.x; x < bottomright.x; ++x) {
            screen_buffer.set_pixel(Vec2U(x, y), mp_texture->get_pixel(Vec2U(x, y) + texture_offset));
        }
    }
}

void clgl::Sprite::draw_no_clipping_transformed(ScreenBuffer &screen_buffer) {
    F32 angle_in_radians = m_rotation * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos = std::cos(angle_in_radians);
    F32 angle_sin = std::sin(angle_in_radians);

    Vec2F transformed_corners[4] {
        { 0.f, 0.f },
        { static_cast<F32>(mp_texture->get_size().x), 0.f },
        { static_cast<F32>(mp_texture->get_size().x), static_cast<F32>(mp_texture->get_size().y) },
        { 0.f, static_cast<F32>(mp_texture->get_size().y) }
    };

    Vec2F topleft = { std::numeric_limits<F32>::max(), std::numeric_limits<F32>::max() };
    Vec2F bottomright = { std::numeric_limits<F32>::min(), std::numeric_limits<F32>::min() };

    for (I32 i = 0u; i < 4; ++i) {
        Vec2F &corner = transformed_corners[i];

        apply_scaling(corner);
        apply_rotation(corner, angle_cos, angle_sin);

        if (corner.x < topleft.x)     topleft.x = corner.x;
        if (corner.y < topleft.y)     topleft.y = corner.y;
        if (corner.x > bottomright.x) bottomright.x = corner.x;
        if (corner.y > bottomright.y) bottomright.y = corner.y;
    }

    Vec2I size = bottomright - topleft;

    #pragma omp parallel for
    for (I32 y = 0; y <= size.y; ++y) {
        for (I32 x = 0; x <= size.x; ++x) {
            Vec2F destination_coordinates = Vec2F(x, y) + topleft;

            Vec2F translated = destination_coordinates;

            apply_inverse_rotation(translated, angle_cos, angle_sin);
            apply_inverse_scaling(translated);

            if (
                translated.x < 0.f ||
                translated.y < 0.f ||
                translated.x >= static_cast<F32>(mp_texture->get_size().x) ||
                translated.y >= static_cast<F32>(mp_texture->get_size().y)
                ) continue;

            Pixel pixel;
            screen_buffer.get_current_interpolator()(translated, *mp_texture, pixel);

            apply_translation(destination_coordinates);

            destination_coordinates = {
                std::floorf(destination_coordinates.x),
                std::floorf(destination_coordinates.y)
            };

            screen_buffer.set_pixel(destination_coordinates, pixel);
        }
    }
}

void clgl::Sprite::draw_no_clipping_nontransformed(ScreenBuffer &screen_buffer) {
    Vec2I topleft = Vec2I(get_position() - get_origin());
    Vec2I bottomright = topleft + mp_texture->get_size();

    Vec2I texture_offset = topleft * -1.f;

    #pragma omp parallel for
    for (I32 y = topleft.y; y < bottomright.y; ++y) {
        for (I32 x = topleft.x; x < bottomright.x; ++x) {
            screen_buffer.set_pixel(Vec2U(x, y), mp_texture->get_pixel(Vec2U(x, y) + texture_offset));
        }
    }
}