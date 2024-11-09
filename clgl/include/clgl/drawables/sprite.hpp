#pragma once

#include "clgl/drawable.hpp"
#include "clgl/transformable.hpp"
#include "clgl/texture.hpp"
#include <memory>
#include <functional>

namespace clgl
{
class Sprite : public Drawable, public Transformable
{
public:

    void draw(ScreenBuffer &screen_buffer) override;
	void draw_no_clipping(ScreenBuffer &screen_buffer) override;

    void set_texture(std::shared_ptr<Texture> texture);

private:

    Vec2F m_topleft { 0.f, 0.f };

    std::shared_ptr<Texture> mp_texture = nullptr;
};

namespace utils
{
    // this function assumes that the coordinates are contained inside the texture
    void interpolate_pixel(const Vec2F &coordinates, Pixel &output, const Texture &texture);
}
}