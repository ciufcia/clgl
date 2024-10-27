#include "clgl/drawables/intcircle.hpp"
#include <algorithm>

clgl::IntCircle::IntCircle(const Vec2I &center, I32 radius, bool filled, const Pixel &pixel) {
    this->center = center; this->radius = radius; this->filled = filled; this->pixel = pixel;
}

void clgl::IntCircle::draw(ScreenBuffer &screen_buffer) {
    I32 current_x = 0;
    I32 current_y = radius;
    I32 decision  = 3 - (2 * radius);

    while (current_x <= current_y) {
        if (filled) draw_in_all_octants_filled(screen_buffer, {current_x, current_y});
        else        draw_in_all_octants(screen_buffer, {current_x, current_y});

        if (decision > 0) {
            --current_y;
            decision += 4 * (current_x - current_y) + 10;
        } else {
            decision += 4 * current_x + 6;
        }

        ++current_x;
    }
}

void clgl::IntCircle::draw_no_clipping(ScreenBuffer &screen_buffer) {
    I32 current_x = 0;
    I32 current_y = radius;
    I32 decision  = 3 - (2 * radius);

    while (current_x <= current_y) {
        if (filled) draw_in_all_octants_filled_no_clipping(screen_buffer, {current_x, current_y});
        else        draw_in_all_octants_no_clipping(screen_buffer, {current_x, current_y});

        if (decision > 0) {
            --current_y;
            decision += 4 * (current_x - current_y) + 10;
        } else {
            decision += 4 * current_x + 6;
        }

        ++current_x;
    }
}

void clgl::IntCircle::move(const Vec2F &offset) {
    center += offset;
}

void clgl::IntCircle::draw_in_all_octants(ScreenBuffer &screen_buffer, const Vec2I &point) {
    // going from 1st octant to the 8th
    screen_buffer.set_pixel_safe(Vec2I{center.x + point.x, center.y + point.y}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{center.x + point.y, center.y + point.x}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{center.x + point.y, center.y - point.x}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{center.x + point.x, center.y - point.y}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{center.x - point.x, center.y - point.y}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{center.x - point.y, center.y - point.x}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{center.x - point.y, center.y + point.x}, pixel);
    screen_buffer.set_pixel_safe(Vec2I{center.x - point.x, center.y + point.y}, pixel);
}

void clgl::IntCircle::draw_in_all_octants_filled(ScreenBuffer &screen_buffer, const Vec2I &point) {
    Vec2I max   = Vec2I(screen_buffer.get_max_int_coordinates());
    ++max.x;

    I32 x_start = std::clamp(center.x - point.x, 0, max.x);
    I32 x_end   = std::clamp(center.x + point.x + 1, 0, max.x);

    // 1st and 8th octants
    I32 y = center.y + point.y;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    }

    // 4th and 5th octants
    y = center.y - point.y;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    }

    x_start = std::clamp(center.x - point.y, 0, max.x);
    x_end   = std::clamp(center.x + point.y + 1, 0, max.x);

    // 2nd and 7th octants
    y = center.y + point.x;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    }

    // 3rd and 6th octants
    y = center.y - point.x;
    if (y >= 0 && y <= max.y) {
        for (I32 x = x_start; x < x_end; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    }
}

void clgl::IntCircle::draw_in_all_octants_no_clipping(ScreenBuffer &screen_buffer, const Vec2I &point) {
    // going from 1st octant to the 8th
    screen_buffer.set_pixel(Vec2I{center.x + point.x, center.y + point.y}, pixel);
    screen_buffer.set_pixel(Vec2I{center.x + point.y, center.y + point.x}, pixel);
    screen_buffer.set_pixel(Vec2I{center.x + point.y, center.y - point.x}, pixel);
    screen_buffer.set_pixel(Vec2I{center.x + point.x, center.y - point.y}, pixel);
    screen_buffer.set_pixel(Vec2I{center.x - point.x, center.y - point.y}, pixel);
    screen_buffer.set_pixel(Vec2I{center.x - point.y, center.y - point.x}, pixel);
    screen_buffer.set_pixel(Vec2I{center.x - point.y, center.y + point.x}, pixel);
    screen_buffer.set_pixel(Vec2I{center.x - point.x, center.y + point.y}, pixel);
}

void clgl::IntCircle::draw_in_all_octants_filled_no_clipping(ScreenBuffer &screen_buffer, const Vec2I &point) {
    // 1st and 8th octants
    I32 y = center.y + point.y;
    for (I32 x = center.x - point.x; x <= center.x + point.x; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    // 2nd and 7th octants
    y = center.y + point.x;
    for (I32 x = center.x - point.y; x <= center.x + point.y; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    // 3rd and 6th octants
    y = center.y - point.x;
    for (I32 x = center.x - point.y; x <= center.x + point.y; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
    // 4th and 5th octants
    y = center.y - point.y;
    for (I32 x = center.x - point.x; x <= center.x + point.x; ++x) screen_buffer.set_pixel(Vec2I{x, y}, pixel);
}