#pragma once

#include "utils/utils.hpp"
#include "screen_writer.hpp"
#include "screen_buffer.hpp"
#include "color.hpp"
#include <memory>
#include "clgl_resource_manager.hpp"

namespace clgl
{
class Screen;

class Drawer
{
public:

                 Drawer() = default;
    virtual      ~Drawer() = default;

    virtual void run(const ScreenBuffer &screen_buffer, ScreenWriter &screen_writer) = 0;

    template<typename DrawerType, typename ... Params> requires std::derived_from<DrawerType, Drawer>
    [[nodiscard]] static std::shared_ptr<DrawerType> create(Params&... params);

protected:

    virtual void on_registered() {};
    virtual void on_set() {};
    virtual void on_unset() {};

    [[nodiscard]] virtual CLGLResourceManager * get_resource_manager() const final;

private:

    CLGLResourceManager *mp_resource_manager = nullptr;

friend class Screen;
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