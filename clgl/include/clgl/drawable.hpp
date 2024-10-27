#pragma once

#include "screen_buffer.hpp"

namespace clgl
{
class Drawable
{
public:

	virtual      ~Drawable()                                   = default;

	virtual void draw(ScreenBuffer &screen_buffer)             = 0;
	virtual void draw_no_clipping(ScreenBuffer &screen_buffer) = 0;
};
}