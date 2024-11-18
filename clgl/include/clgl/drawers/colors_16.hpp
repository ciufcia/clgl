#pragma once

#include "clgl/drawer.hpp"

namespace clgl::drawers
{
class Colors16 final : public Drawer
{
public:

    Colors16() = default;

    virtual void run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) override;

protected:

    virtual void on_set(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) override;
public:

    CLGLResourceID color_mappings_id = 0u;
    
private:

    const ColorMappings *p_color_mappings = nullptr;
};
}