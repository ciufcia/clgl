#include "clgl/drawables/intline.hpp"
#include <cmath>

clgl::IntLine::IntLine(const Vec2I &end1, const Vec2I &end2, const Pixel &pixel) {
    this->end1 = end1; this->end2 = end2; this->pixel = pixel;
}

void clgl::IntLine::draw(ScreenBuffer &screen_buffer) {
    utils::LineClippingData clipping_data = utils::clip_line(end1, end2, { 0.f, 0.f }, screen_buffer.get_max_float_coordinates());

    if (!clipping_data.visible) return;

    Vec2I start = Vec2I(clipping_data.end1.x, clipping_data.end1.y);
    Vec2I end   = Vec2I(clipping_data.end2.x, clipping_data.end2.y);

    draw(screen_buffer, start, end);
}

void clgl::IntLine::draw_no_clipping(ScreenBuffer &screen_buffer) {
    draw(screen_buffer, end1, end2);
}

void clgl::IntLine::move(const Vec2I &offset) {
    end1 += offset;
    end2 += offset;
}

void clgl::IntLine::draw(ScreenBuffer &screen_buffer, Vec2I end1, Vec2I end2) {
    Vec2I start = Vec2I(end1.x, end1.y);
    Vec2I end   = Vec2I(end2.x, end2.y);     

    bool steep = false;

    if (abs(end.x - start.x) < abs(end.y - start.y)) {
        std::swap(start.x, start.y);
        std::swap(end.x, end.y);
        steep = true;
    }

    if (start.x > end.x) {
        std::swap(start.x, end.x);
        std::swap(start.y, end.y);
    }

    I32 dx           = end.x - start.x;
    I32 dy           = end.y - start.y;

    I32 cy           = start.y;
    I32 y_step       = (end.y > start.y) ? 1 : -1;

    I32 error_change = abs(dy) * 2;
    I32 error        = 0;

    for (I32 cx = start.x; cx <= end.x; ++cx) {
        if (steep) screen_buffer.set_pixel(Vec2U(cy, cx), pixel);
        else       screen_buffer.set_pixel(Vec2U(cx, cy), pixel);

        error += error_change;
        if (error > dx) {
            cy    += y_step;
            error -= dx * 2;
        }
    }
}