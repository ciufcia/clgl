#pragma once

#include "clgl/drawer.hpp"

namespace clgl::drawers
{
class FullRGB final : public Drawer
{
public:

    FullRGB() = default;

    virtual void run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) override;
};
}