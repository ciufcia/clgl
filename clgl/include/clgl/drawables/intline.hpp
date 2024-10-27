#pragma once

#include "clgl/drawables/line.hpp"

namespace clgl
{
struct IntLine : public Drawable
{
         IntLine() = default;
         IntLine(const Vec2I &end1, const Vec2I &end2, const Pixel &pixel);

    void draw(ScreenBuffer &screen_buffer) override;
    void draw_no_clipping(ScreenBuffer &screen_buffer) override;
    void move(const Vec2I &offset);

    Vec2I end1;
    Vec2I end2;
    Pixel pixel;

private:

    void draw(ScreenBuffer &screen_buffer, Vec2I start, Vec2I end);
};
}