#pragma once

#include "clgl/drawer.hpp"
#include "clgl/shading_palettes.hpp"
#include <memory>

namespace clgl::drawers
{
class Colors16GrayscaleShaded final : public Drawer
{
public:

    Colors16GrayscaleShaded() = default;

    void                              set_shading_palette(const std::wstring &shading_palette);
    [[nodiscard]] const std::wstring &get_shading_palette() const;

    virtual void run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) override;

protected:

    virtual void on_set() override;

private:

    const ColorMappings *p_color_mappings = nullptr;

    U8 m_grayscale_colors[4] = {
        static_cast<U8>(utils::terminal_16_colors::ForegroundColor::Black),
        static_cast<U8>(utils::terminal_16_colors::ForegroundColor::Gray),
        static_cast<U8>(utils::terminal_16_colors::ForegroundColor::LightGray),
        static_cast<U8>(utils::terminal_16_colors::ForegroundColor::White)
    };

    std::wstring m_shading_palette = shading_palettes::default_palette_4;
};
}