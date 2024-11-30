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

    void draw_transformed(ScreenBuffer &screen_buffer);
    void draw_no_clipping_transformed(ScreenBuffer &screen_buffer);
    void draw_nontransformed(ScreenBuffer &screen_buffer);
    void draw_no_clipping_nontransformed(ScreenBuffer &screen_buffer);

private:

    Vec2F m_topleft { 0.f, 0.f };

    std::shared_ptr<Texture> mp_texture = nullptr;
};
}