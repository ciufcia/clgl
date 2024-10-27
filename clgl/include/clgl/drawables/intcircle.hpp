#pragma once

#include "clgl/drawable.hpp"

namespace clgl
{
class IntCircle : public Drawable 
{
public:

         IntCircle() = default;
         IntCircle(const Vec2I &center, I32 radius, bool filled, const Pixel &pixel);

    void draw(ScreenBuffer &screen_buffer) override;
    void draw_no_clipping(ScreenBuffer &screen_buffer) override;
    void move(const Vec2F &offset);

    Vec2I center { 0, 0 };
    I32   radius = 1;
    Pixel pixel;
    bool  filled  = false;

private:

    void draw_in_all_octants(ScreenBuffer &screen_buffer, const Vec2I &point);
    void draw_in_all_octants_filled(ScreenBuffer &screen_buffer, const Vec2I &point);
    void draw_in_all_octants_no_clipping(ScreenBuffer &screen_buffer, const Vec2I &point);
    void draw_in_all_octants_filled_no_clipping(ScreenBuffer &screen_buffer, const Vec2I &point);
};
}