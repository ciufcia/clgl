#pragma once

#include "clgl/drawer.hpp"
#include "clgl/shading_palettes.hpp"

namespace clgl::drawers
{
class SingleColor16Shaded final : public Drawer
{
public:

    SingleColor16Shaded() = default;

    void                              set_shading_palette(const std::wstring &shading_palette);
    [[nodiscard]] const std::wstring &get_shading_palette() const;

    virtual void run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) override;

protected:

    virtual void on_set(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) override;

public:

    Color color { 0u, 0u, 0u };
    CLGLResourceID color_mappings_id = 0u;
    
private:

    const ColorMappings *p_color_mappings = nullptr;
    
    std::wstring m_shading_palette = shading_palettes::default_palette_16;
};
}