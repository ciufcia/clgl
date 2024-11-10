#pragma once

#include "clgl/drawer.hpp"

namespace clgl::drawers
{
class Colors16Grayscale final : public Drawer
{
public:

    Colors16Grayscale() = default;

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
};
}