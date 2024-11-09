#include "clgl/drawables/sprite.hpp"
#include <numbers>
#include "clgl/utils/utils.hpp"
#include <cmath>
#include <algorithm>
#include "clgl/drawables/intline.hpp"

void clgl::Sprite::draw(ScreenBuffer &screen_buffer) {
    F32 angle_in_radians = m_rotation * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos        = std::cos(angle_in_radians);
    F32 angle_sin        = std::sin(angle_in_radians);

    Vec2F transformed_corners[4] {
        { 0.f, 0.f },
        { static_cast<F32>(mp_texture->get_size().x), 0.f },
        { static_cast<F32>(mp_texture->get_size().x), static_cast<F32>(mp_texture->get_size().y) },
        { 0.f, static_cast<F32>(mp_texture->get_size().y) }
    };

    Vec2F topleft     = { std::numeric_limits<F32>::max(), std::numeric_limits<F32>::max() };
    Vec2F bottomright = { std::numeric_limits<F32>::min(), std::numeric_limits<F32>::min() };

    for (I32 i = 0u; i < 4; ++i) {
        Vec2F &corner = transformed_corners[i];

        apply_scaling(corner);
        apply_rotation(corner, angle_cos, angle_sin);

        if (corner.x < topleft.x)     topleft.x     = corner.x;
        if (corner.y < topleft.y)     topleft.y     = corner.y;
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
            utils::interpolate_pixel(translated, pixel, *mp_texture);

            apply_translation(destination_coordinates);

            screen_buffer.set_pixel_safe(destination_coordinates, pixel);
        }
    }
}

void clgl::Sprite::draw_no_clipping(ScreenBuffer &screen_buffer) {
    F32 angle_in_radians = m_rotation * static_cast<F32>(std::numbers::pi) * utils::inverse_180;

    F32 angle_cos        = std::cos(angle_in_radians);
    F32 angle_sin        = std::sin(angle_in_radians);

    Vec2F transformed_corners[4] {
        { 0.f, 0.f },
        { static_cast<F32>(mp_texture->get_size().x), 0.f },
        { static_cast<F32>(mp_texture->get_size().x), static_cast<F32>(mp_texture->get_size().y) },
        { 0.f, static_cast<F32>(mp_texture->get_size().y) }
    };

    Vec2F topleft     = { std::numeric_limits<F32>::max(), std::numeric_limits<F32>::max() };
    Vec2F bottomright = { std::numeric_limits<F32>::min(), std::numeric_limits<F32>::min() };

    for (I32 i = 0u; i < 4; ++i) {
        Vec2F &corner = transformed_corners[i];

        apply_scaling(corner);
        apply_rotation(corner, angle_cos, angle_sin);

        if (corner.x < topleft.x)     topleft.x     = corner.x;
        if (corner.y < topleft.y)     topleft.y     = corner.y;
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
            utils::interpolate_pixel(translated, pixel, *mp_texture);

            apply_translation(destination_coordinates);

            screen_buffer.set_pixel(destination_coordinates, pixel);
        }
    }
}

void clgl::Sprite::set_texture(std::shared_ptr<Texture> texture) {
    mp_texture = texture;
}

void clgl::utils::interpolate_pixel(const Vec2F &coordinates, Pixel &output, const Texture &texture) {
    Color topleft_color, topright_color, bottomleft_color, bottomright_color;

    Vec2F topleft_pixel_topleft     { std::floor(coordinates.x), std::floor(coordinates.y) };

    Vec2I topleft_pixel_coordinates = topleft_pixel_topleft;
    I32 topleft_pixel_index         = topleft_pixel_coordinates.x + topleft_pixel_coordinates.y * texture.get_size().x;

    topleft_color                   = texture.get_pixel(topleft_pixel_index).color;

    bool right_in                   = topleft_pixel_topleft.x + 1.f < static_cast<F32>(texture.get_size().x);
    bool bottom_in                  = topleft_pixel_topleft.y + 1.f < static_cast<F32>(texture.get_size().y);

    if (right_in && bottom_in) {
        topright_color    = texture.get_pixel(topleft_pixel_index + 1).color;
        bottomleft_color  = texture.get_pixel(topleft_pixel_index + texture.get_size().x).color;
        bottomright_color = texture.get_pixel(topleft_pixel_index + texture.get_size().x + 1).color;
    } else if (right_in) {
        topright_color    = texture.get_pixel(topleft_pixel_index + 1).color;
        bottomleft_color  = topleft_color;
        bottomright_color = Color(
            static_cast<U8>(((static_cast<U32>(topleft_color.r) + static_cast<U32>(topright_color.r))) >> 1),
            static_cast<U8>(((static_cast<U32>(topleft_color.g) + static_cast<U32>(topright_color.g))) >> 1),
            static_cast<U8>(((static_cast<U32>(topleft_color.b) + static_cast<U32>(topright_color.b))) >> 1)
        );
    } else if (bottom_in) {
        topright_color    = topleft_color;
        bottomleft_color  = texture.get_pixel(topleft_pixel_index + texture.get_size().x).color;
        bottomright_color = Color(
            static_cast<U8>(((static_cast<U32>(topleft_color.r) + static_cast<U32>(bottomleft_color.r))) >> 1),
            static_cast<U8>(((static_cast<U32>(topleft_color.g) + static_cast<U32>(bottomleft_color.g))) >> 1),
            static_cast<U8>(((static_cast<U32>(topleft_color.b) + static_cast<U32>(bottomleft_color.b))) >> 1)
        );
    } else {
        topright_color    = topleft_color;
        bottomleft_color  = topleft_color;
        bottomright_color = topleft_color;
    }

    F32 fx = coordinates.x - topleft_pixel_topleft.x;
    F32 fy = coordinates.y - topleft_pixel_topleft.y;

    F32 topleft_weight     = (1.f - fx) * (1.f - fy);
    F32 topright_weight    = fx * (1.f - fy);
    F32 bottomleft_weight  = (1.f - fx) * fy;
    F32 bottomright_weight = fx * fy;

    output.color = {
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

    output.character = texture.get_pixel(topleft_pixel_index).character;
}