#pragma once

#include "clgl/drawable.hpp"

namespace clgl
{
class Circle : public Drawable 
{
public:

         Circle() = default;
         Circle(const Vec2F &center, F32 radius, bool filled, const Pixel &pixel);

    void draw(ScreenBuffer &screen_buffer) override;
    void draw_no_clipping(ScreenBuffer &screen_buffer) override;
    void move(const Vec2F &offset);

    Vec2F center  { 0.f, 0.f };
    F32   radius  = 1.f;
    Pixel pixel;
    bool  filled  = false;

private:

    void draw_with_overdraw(ScreenBuffer &screen_buffer);
    void draw_no_clipping_with_overdraw(ScreenBuffer &screen_buffer);
    void draw_without_overdraw(ScreenBuffer &screen_buffer);
    void draw_no_clipping_without_overdraw(ScreenBuffer &screen_buffer);

    void draw_in_all_octants_with_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point);
    void draw_in_all_octants_filled_with_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point);
    void draw_in_all_octants_no_clipping_with_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point);
    void draw_in_all_octants_filled_no_clipping_with_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point);

    void draw_in_all_octants_without_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point);
    void draw_in_all_octants_filled_without_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point);
    void draw_in_all_octants_no_clipping_without_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point);
    void draw_in_all_octants_filled_no_clipping_without_overdraw(ScreenBuffer &screen_buffer, const Vec2F &point);
};

namespace utils
{
constexpr F32 circle_drawing_shift = -0.5f;
}
}