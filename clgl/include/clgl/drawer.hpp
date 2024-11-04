#pragma once

#include "utils/utils.hpp"
#include "screen_writer.hpp"
#include "screen_buffer.hpp"
#include "color.hpp"
#include <memory>

namespace clgl
{
class Drawer
{
public:

                 Drawer() = default;
    virtual      ~Drawer() = default;

    virtual void run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer, const ColorMappings &color_mappings) = 0;

    template<typename DrawerType, typename ... Params> requires std::derived_from<DrawerType, Drawer>
    [[nodiscard]] static std::shared_ptr<DrawerType> create(Params&... params);
};

namespace utils
{
void get_ANSI_color(std::wstring &output, Color color);
void get_ANSI_color_reset(std::wstring &output);
void color_with_ANSI(std::wstring &output, const std::wstring &character, Color color);
}

template<typename DrawerType, typename ... Params> requires std::derived_from<DrawerType, Drawer>
static std::shared_ptr<DrawerType> Drawer::create(Params&... params) {
    std::shared_ptr<DrawerType> p_drawer = std::make_shared<DrawerType>(params...);
    return p_drawer;
}
}