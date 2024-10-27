#pragma once

#include "clgl/drawable.hpp"

namespace clgl
{
struct Line : public Drawable
{
         Line() = default;
         Line(const Vec2F &end1, const Vec2F &end2, const Pixel &pixel);

    void draw(ScreenBuffer &screen_buffer) override;
    void draw_no_clipping(ScreenBuffer &screen_buffer) override;
    void move(const Vec2F &offset);

    Vec2F end1;
    Vec2F end2;
    Pixel pixel;

private:

    void draw(ScreenBuffer &screen_buffer, Vec2F start, Vec2F end);
};

namespace utils
{
struct LineClippingData
{
    bool  visible = true;

    Vec2F end1;
    Vec2F end2;
};

LineClippingData clip_line(const Vec2F &end1, const Vec2F &end2, const Vec2F &top_left, const Vec2F &bottom_right);

constexpr U8     cohen_sutherland_inside_region = 0b00000000;
constexpr U8     cohen_sutherland_left_region   = 0b00001000;
constexpr U8     cohen_sutherland_top_region    = 0b00000100;
constexpr U8     cohen_sutherland_right_region  = 0b00000010;
constexpr U8     cohen_sutherland_bottom_region = 0b00000001;

U8               assign_cohen_sutherland_region_codes(const Vec2F &point, const Vec2F &top_left, const Vec2F &bottom_right);
}
}