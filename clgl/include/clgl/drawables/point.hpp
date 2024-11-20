#pragma once

#include "clgl/pixel.hpp"
#include "clgl/drawable.hpp"

namespace clgl
{
struct Point : public Drawable
{
    Point() = default;
    Point(const Vec2F &position, const Pixel &pixel);

    Pixel pixel;
    Vec2F position;

    void draw(ScreenBuffer &screen_buffer) override;
    void draw_no_clipping(ScreenBuffer &screen_buffer) override;
    void move(const Vec2F &move);
};
}