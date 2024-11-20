#include "clgl/drawables/point.hpp"

clgl::Point::Point(const Vec2F &position, const Pixel &pixel) {
    this->pixel = pixel; this->position = position;
}

void clgl::Point::draw(ScreenBuffer &screen_buffer) {
    screen_buffer.set_pixel_safe(position, pixel);
}

void clgl::Point::draw_no_clipping(ScreenBuffer &screen_buffer) {
    screen_buffer.set_pixel(position, pixel);
}

void clgl::Point::move(const Vec2F &offset) {
    position += offset;
}