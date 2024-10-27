#pragma once

#include "clgl/drawer.hpp"

namespace clgl::drawers
{
class Colors16 final : public Drawer
{
public:

    Colors16() = default;

    virtual void run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer, const ColorMappings &color_mappings) override;
};
}