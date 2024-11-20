#include "clgl/drawables/circle.hpp"
#include "clgl/utils/utils.hpp"
#include <algorithm>

clgl::Circle::Circle(const Vec2F &center, F32 radius, bool filled, const Pixel &pixel) {
    this->center = center; this->radius = radius; this->filled = filled; this->pixel = pixel;
}

void clgl::Circle::draw(ScreenBuffer &screen_buffer) {
    if (screen_buffer.is_color_blending_enabled() && pixel.color.a == 255u)
        draw_with_overdraw(screen_buffer);
    else
        draw_without_overdraw(screen_buffer);
}

void clgl::Circle::draw_no_clipping(ScreenBuffer &screen_buffer) {
    if (!screen_buffer.is_color_blending_enabled() || pixel.color.a == 255u)
        draw_no_clipping_with_overdraw(screen_buffer);
    else
        draw_no_clipping_without_overdraw(screen_buffer);
}

void clgl::Circle::move(const Vec2F &offset) {
    center += offset;
}

void clgl::Circle::draw_with_overdraw(ScreenBuffer &screen_buffer) {
    F32 current_x = 0.f;
    F32 current_y = radius;

    F32 decision = 1.25f - radius;

    while (current_x <= current_y) {
        if (filled) draw_in_all_octants_filled_with_overdraw(screen_buffer, { current_x, current_y });
        else        draw_in_all_octants_with_overdraw(screen_buffer, { current_x, current_y });

        if (decision <= 0.f) {
            decision += 2 * current_x + 1;
        } else {
            --current_y;
            decision += 2 * current_x - 2 * current_y + 1.25f;
        }

        ++current_x;   
    }
}

void clgl::Circle::draw_no_clipping_with_overdraw(ScreenBuffer &screen_buffer) {
    F32 current_x = 0.f;
    F32 current_y = radius;

    F32 decision = 1.25f - radius;

    while (current_x <= current_y) {
        U32 index = screen_buffer.get_buffer_index(Vec2U(current_x, current_y));

        if (filled) draw_in_all_octants_filled_no_clipping_with_overdraw(screen_buffer, { current_x, current_y} );
        else        draw_in_all_octants_no_clipping_with_overdraw(screen_buffer, { current_x, current_y});

        if (decision <= 0.f) {
            decision += 2 * current_x + 1;
        } else {
            --current_y;
            decision += 2 * current_x - 2 * current_y + 1.25f;
        }

        ++current_x;   
    }

    screen_buffer.clear_changed_flags(constants::overdraw_flag);
}

void clgl::Circle::draw_without_overdraw(ScreenBuffer &screen_buffer) {
    F32 current_x = 0.f;
    F32 current_y = radius;

    F32 decision = 1.25f - radius;

    while (current_x <= current_y) {
        if (filled) draw_in_all_octants_filled_without_overdraw(screen_buffer, { current_x, current_y });
        else        draw_in_all_octants_without_overdraw(screen_buffer, { current_x, current_y });

        if (decision <= 0.f) {
            decision += 2 * current_x + 1;
        } else {
            --current_y;
            decision += 2 * current_x - 2 * current_y + 1.25f;
        }

        ++current_x;   
    }

    screen_buffer.clear_changed_flags(constants::overdraw_flag);
}

void clgl::Circle::draw_no_clipping_without_overdraw(ScreenBuffer &screen_buffer) {
    F32 current_x = 0.f;
    F32 current_y = radius;

    F32 decision = 1.25f - radius;

    while (current_x <= current_y) {
        if (filled) draw_in_all_octants_filled_no_clipping_without_overdraw(screen_buffer, { current_x, current_y });
        else        draw_in_all_octants_no_clipping_without_overdraw(screen_buffer, { current_x, current_y });

        if (decision <= 0.f) {
            decision += 2 * current_x + 1;
        } else {
            --current_y;
            decision += 2 * current_x - 2 * current_y + 1.25f;
        }

        ++current_x;   
    }
}

void clgl::Circle::draw_in_all_octants_with_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point) {
    // going from 1st octant to the 8th
    Vec2F shifted_point { point.x + utils::circle_drawing_shift, point.y + utils::circle_drawing_shift };

    screen_buffer.set_pixel_safe(Vec2I{static_cast<I32>(center.x + shifted_point.x), static_cast<I32>(center.y + shifted_point.y)}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{static_cast<I32>(center.x + shifted_point.y), static_cast<I32>(center.y + shifted_point.x)}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{static_cast<I32>(center.x + shifted_point.y), static_cast<I32>(center.y - shifted_point.x)}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{static_cast<I32>(center.x + shifted_point.x), static_cast<I32>(center.y - shifted_point.y)}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{static_cast<I32>(center.x - shifted_point.x), static_cast<I32>(center.y - shifted_point.y)}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{static_cast<I32>(center.x - shifted_point.y), static_cast<I32>(center.y - shifted_point.x)}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{static_cast<I32>(center.x - shifted_point.y), static_cast<I32>(center.y + shifted_point.x)}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{static_cast<I32>(center.x - shifted_point.x), static_cast<I32>(center.y + shifted_point.y)}, pixel);
}

void clgl::Circle::draw_in_all_octants_filled_with_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point) {
    Vec2F shifted_point { point.x + utils::circle_drawing_shift, point.y + utils::circle_drawing_shift };

    Vec2I max   = Vec2I(screen_buffer.get_max_int_coordinates());
    ++max.x;

    I32 x_start = std::clamp(static_cast<I32>(center.x - shifted_point.x), 0, max.x);
    I32 x_end   = std::clamp(static_cast<I32>(center.x + shifted_point.x) + 1, 0, max.x);

    // 1st and 8th octants
    I32 y = center.y + shifted_point.y;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    }

    // 4th and 5th octants
    y = center.y - shifted_point.y;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    }

    x_start = std::clamp(static_cast<I32>(center.x - shifted_point.y), 0, max.x);
    x_end   = std::clamp(static_cast<I32>(center.x + shifted_point.y) + 1, 0, max.x);

    // 2nd and 7th octants
    y = center.y + shifted_point.x;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    }

    // 3rd and 6th octants
    y = center.y - shifted_point.x;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    }
}

void clgl::Circle::draw_in_all_octants_no_clipping_with_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point) {
    Vec2F shifted_point { point.x + utils::circle_drawing_shift, point.y + utils::circle_drawing_shift };

    // going from 1st octant to the 8th
    screen_buffer.set_pixel(Vec2I{static_cast<I32>(center.x + shifted_point.x), static_cast<I32>(center.y + shifted_point.y)}, pixel);
    screen_buffer.set_pixel(Vec2I{static_cast<I32>(center.x + shifted_point.y), static_cast<I32>(center.y + shifted_point.x)}, pixel);
    screen_buffer.set_pixel(Vec2I{static_cast<I32>(center.x + shifted_point.y), static_cast<I32>(center.y - shifted_point.x)}, pixel);
    screen_buffer.set_pixel(Vec2I{static_cast<I32>(center.x + shifted_point.x), static_cast<I32>(center.y - shifted_point.y)}, pixel);
    screen_buffer.set_pixel(Vec2I{static_cast<I32>(center.x - shifted_point.x), static_cast<I32>(center.y - shifted_point.y)}, pixel);
    screen_buffer.set_pixel(Vec2I{static_cast<I32>(center.x - shifted_point.y), static_cast<I32>(center.y - shifted_point.x)}, pixel);
    screen_buffer.set_pixel(Vec2I{static_cast<I32>(center.x - shifted_point.y), static_cast<I32>(center.y + shifted_point.x)}, pixel);
    screen_buffer.set_pixel(Vec2I{static_cast<I32>(center.x - shifted_point.x), static_cast<I32>(center.y + shifted_point.y)}, pixel);
}

void clgl::Circle::draw_in_all_octants_filled_no_clipping_with_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point) {
    Vec2F shifted_point { point.x + utils::circle_drawing_shift, point.y + utils::circle_drawing_shift };

    // 1st and 8th octants
    I32 y = static_cast<I32>(center.y + shifted_point.y);
    for (I32 x = center.x - shifted_point.x; x <= center.x + shifted_point.x; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    // 2nd and 7th octants
    y = static_cast<I32>(center.y + shifted_point.x);
    for (I32 x = center.x - shifted_point.y; x <= center.x + shifted_point.y; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    // 3rd and 6th octants
    y = static_cast<I32>(center.y - shifted_point.x);
    for (I32 x = center.x - shifted_point.y; x <= center.x + shifted_point.y; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    // 4th and 5th octants
    y = static_cast<I32>(center.y - shifted_point.y);
    for (I32 x = center.x - shifted_point.x; x <= center.x + shifted_point.x; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
}

void clgl::Circle::draw_in_all_octants_without_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point) {
    // going from 1st octant to the 8th
    Vec2F shifted_point { point.x + utils::circle_drawing_shift, point.y + utils::circle_drawing_shift };

    screen_buffer.set_pixel_safe_without_overdraw(Vec2I{static_cast<I32>(center.x + shifted_point.x), static_cast<I32>(center.y + shifted_point.y)}, pixel);
    screen_buffer.set_pixel_safe_without_overdraw(Vec2I{static_cast<I32>(center.x + shifted_point.y), static_cast<I32>(center.y + shifted_point.x)}, pixel);
    screen_buffer.set_pixel_safe_without_overdraw(Vec2I{static_cast<I32>(center.x + shifted_point.y), static_cast<I32>(center.y - shifted_point.x)}, pixel);
    screen_buffer.set_pixel_safe_without_overdraw(Vec2I{static_cast<I32>(center.x + shifted_point.x), static_cast<I32>(center.y - shifted_point.y)}, pixel);
    screen_buffer.set_pixel_safe_without_overdraw(Vec2I{static_cast<I32>(center.x - shifted_point.x), static_cast<I32>(center.y - shifted_point.y)}, pixel);
    screen_buffer.set_pixel_safe_without_overdraw(Vec2I{static_cast<I32>(center.x - shifted_point.y), static_cast<I32>(center.y - shifted_point.x)}, pixel);
    screen_buffer.set_pixel_safe_without_overdraw(Vec2I{static_cast<I32>(center.x - shifted_point.y), static_cast<I32>(center.y + shifted_point.x)}, pixel);
    screen_buffer.set_pixel_safe_without_overdraw(Vec2I{static_cast<I32>(center.x - shifted_point.x), static_cast<I32>(center.y + shifted_point.y)}, pixel);
}

void clgl::Circle::draw_in_all_octants_filled_without_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point) {
    Vec2F shifted_point { point.x + utils::circle_drawing_shift, point.y + utils::circle_drawing_shift };

    Vec2I max   = Vec2I(screen_buffer.get_max_int_coordinates());
    ++max.x;

    I32 x_start = std::clamp(static_cast<I32>(center.x - shifted_point.x), 0, max.x);
    I32 x_end   = std::clamp(static_cast<I32>(center.x + shifted_point.x) + 1, 0, max.x);

    // 1st and 8th octants
    I32 y = center.y + shifted_point.y;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel_without_overdraw(Vec2I{x, y}, pixel);
    }

    // 4th and 5th octants
    y = center.y - shifted_point.y;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel_without_overdraw(Vec2I{x, y}, pixel);
    }

    x_start = std::clamp(static_cast<I32>(center.x - shifted_point.y), 0, max.x);
    x_end   = std::clamp(static_cast<I32>(center.x + shifted_point.y) + 1, 0, max.x);

    // 2nd and 7th octants
    y = center.y + shifted_point.x;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel_without_overdraw(Vec2I{x, y}, pixel);
    }

    // 3rd and 6th octants
    y = center.y - shifted_point.x;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel_without_overdraw(Vec2I{x, y}, pixel);
    }
}

void clgl::Circle::draw_in_all_octants_no_clipping_without_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point) {
    Vec2F shifted_point { point.x + utils::circle_drawing_shift, point.y + utils::circle_drawing_shift };

    // going from 1st octant to the 8th
    screen_buffer.set_pixel_without_overdraw(Vec2I{static_cast<I32>(center.x + shifted_point.x), static_cast<I32>(center.y + shifted_point.y)}, pixel);
    screen_buffer.set_pixel_without_overdraw(Vec2I{static_cast<I32>(center.x + shifted_point.y), static_cast<I32>(center.y + shifted_point.x)}, pixel);
    screen_buffer.set_pixel_without_overdraw(Vec2I{static_cast<I32>(center.x + shifted_point.y), static_cast<I32>(center.y - shifted_point.x)}, pixel);
    screen_buffer.set_pixel_without_overdraw(Vec2I{static_cast<I32>(center.x + shifted_point.x), static_cast<I32>(center.y - shifted_point.y)}, pixel);
    screen_buffer.set_pixel_without_overdraw(Vec2I{static_cast<I32>(center.x - shifted_point.x), static_cast<I32>(center.y - shifted_point.y)}, pixel);
    screen_buffer.set_pixel_without_overdraw(Vec2I{static_cast<I32>(center.x - shifted_point.y), static_cast<I32>(center.y - shifted_point.x)}, pixel);
    screen_buffer.set_pixel_without_overdraw(Vec2I{static_cast<I32>(center.x - shifted_point.y), static_cast<I32>(center.y + shifted_point.x)}, pixel);
    screen_buffer.set_pixel_without_overdraw(Vec2I{static_cast<I32>(center.x - shifted_point.x), static_cast<I32>(center.y + shifted_point.y)}, pixel);
}

void clgl::Circle::draw_in_all_octants_filled_no_clipping_without_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point) {
    Vec2F shifted_point { point.x + utils::circle_drawing_shift, point.y + utils::circle_drawing_shift };

    // 1st and 8th octants
    I32 y = static_cast<I32>(center.y + shifted_point.y);
    for (I32 x = center.x - shifted_point.x; x <= center.x + shifted_point.x; ++x) {
        screen_buffer.set_pixel_without_overdraw(Vec2I{x, y}, pixel);
    }
    
    // 2nd and 7th octants
    y = static_cast<I32>(center.y + shifted_point.x);
    for (I32 x = center.x - shifted_point.y; x <= center.x + shifted_point.y; ++x) {
        screen_buffer.set_pixel_without_overdraw(Vec2I{x, y}, pixel);
    }
    // 3rd and 6th octants
    y = static_cast<I32>(center.y - shifted_point.x);
    for (I32 x = center.x - shifted_point.y; x <= center.x + shifted_point.y; ++x) {
        screen_buffer.set_pixel_without_overdraw(Vec2I{x, y}, pixel);
    }
    // 4th and 5th octants
    y = static_cast<I32>(center.y - shifted_point.y);
    for (I32 x = center.x - shifted_point.x; x <= center.x + shifted_point.x; ++x) {
        screen_buffer.set_pixel_without_overdraw(Vec2I{x, y}, pixel);
    }
}