#pragma once

#include "screen_buffer.hpp"
#include "utils/utils.hpp"
#include "font_data.hpp"
#include "screen_writer.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "drawer.hpp"
#include "drawable.hpp"
#include "exceptions.hpp"

namespace clgl
{
class App;

template<typename DrawerType>
struct DrawerInfo
{
    U32                         id;
    std::shared_ptr<DrawerType> pointer;
};

class Screen
{
public:

                                          Screen() = default;

    void                                  fill(const Pixel &pixel);

    void                                  draw(Drawable &drawable);
    void                                  draw_no_clipping(Drawable &drawable);

    [[nodiscard]] const Vec2U &           get_size() const;
    void                                  set_size(const Vec2U &size);

    void                                  set_font(const FontData &font_data);
    
    void                                  set_title(const std::wstring &title);

    template<typename DrawerType, typename ... Params> requires std::derived_from<DrawerType, Drawer>
    DrawerInfo<DrawerType>                register_drawer(Params&... params);

    template<typename DrawerType> requires std::derived_from<DrawerType, Drawer>
    [[nodiscard]] DrawerInfo<DrawerType>  get_drawer();

    [[nodiscard]] std::shared_ptr<Drawer> get_drawer(U32 id);

    template<typename DrawerType> requires std::derived_from<DrawerType, Drawer>
    void                                  set_drawer();

    template<typename DrawerType, typename ... Params> requires std::derived_from<DrawerType, Drawer>
    DrawerInfo<DrawerType>                register_and_set_drawer(Params&... params);

    void                                  set_drawer(U32 id);

    [[nodiscard]] U32                     get_current_drawer_id() const;

private:

    void init(const winapi::Handles &hadles, const Vec2U &size, const FontData &font_data);

    void display();

    void pass_handles(const winapi::Handles &handles);
    void load_color_mappings();
    void set_console_output_mode();
    void hide_cursor();
    void set_color_palette();

    void restore_initial_settings();

    void save_old_console_output_mode();
    void restore_old_console_output_mode();

    void save_old_font();
    void restore_old_font();

    void save_old_size();
    void restore_old_size();

    void save_old_cursor_info();
    void restore_old_cursor_info();

    void save_old_color_palette();
    void restore_old_color_palette();

private:

    winapi::Handles                      m_handles;

    Vec2U                                m_size { 0u, 0u }; //this means that the screen wasnt initialised yet

    FontData                             m_current_font_data;

    ScreenBuffer                         m_screen_buffer;
    ScreenWriter                         m_screen_writer;

    ColorMappings                        m_color_mappings;

    U32                                  m_current_drawer_id = 0u;
    std::shared_ptr<Drawer>              mp_current_drawer = nullptr;

    std::unordered_map<std::size_t, U32> m_drawer_indices;
    std::vector<std::shared_ptr<Drawer>> m_drawers;

    DWORD                                m_old_console_output_mode = 0;

    bool                                 m_first_font_set = false;
    CONSOLE_FONT_INFOEX                  m_old_font_info;

    bool                                 m_first_size_set = false;
    Vec2U                                m_old_size;

    COLORREF                             m_old_color_palette[16];

    CONSOLE_CURSOR_INFO                  m_old_cursor_info;

friend class App;
};

template<typename DrawerType, typename ... Params> requires std::derived_from<DrawerType, Drawer>
DrawerInfo<DrawerType> Screen::register_drawer(Params&... params) {
    const std::size_t hash_code = typeid(DrawerType).hash_code();
    const auto        iter      = m_drawer_indices.find(hash_code);

    if (iter != m_drawer_indices.end()) {
        DrawerInfo<DrawerType> drawer_info = get_drawer<DrawerType>();
        return { drawer_info.id, drawer_info.pointer };
    }

    std::shared_ptr<DrawerType> p_drawer = Drawer::create<DrawerType>(params...);
    const U32                   index = m_drawers.size();

    m_drawers.push_back(p_drawer);
    m_drawer_indices.insert({ hash_code, index });

    return { index, p_drawer };
}

template<typename DrawerType> requires std::derived_from<DrawerType, Drawer>
DrawerInfo<DrawerType> Screen::get_drawer() {
    const std::size_t hash_code = typeid(DrawerType).hash_code();
    const auto        iter      = m_drawer_indices.find(hash_code);

    if (iter == m_drawer_indices.end()) throw exceptions::DrawerNotRegistered();

    std::shared_ptr<Drawer>     p_drawer = m_drawers[iter->second];
    std::shared_ptr<DrawerType> p_drawer_type = std::dynamic_pointer_cast<DrawerType>(p_drawer);

    return { iter->second, p_drawer_type };
}

template<typename DrawerType> requires std::derived_from<DrawerType, Drawer>
void Screen::set_drawer() {
    const std::size_t hash_code = typeid(DrawerType).hash_code();
    const auto        iter      = m_drawer_indices.find(hash_code);

    if (iter == m_drawer_indices.end()) throw exceptions::DrawerNotRegistered();

    m_current_drawer_id = iter->second;
    mp_current_drawer   = m_drawers[m_current_drawer_id];
}

template<typename DrawerType, typename ... Params> requires std::derived_from<DrawerType, Drawer>
DrawerInfo<DrawerType> Screen::register_and_set_drawer(Params&... params) {
    const std::size_t hash_code = typeid(DrawerType).hash_code();
    const auto        iter      = m_drawer_indices.find(hash_code);

    if (iter == m_drawer_indices.end()) register_drawer<DrawerType, Params...>(params...);

    set_drawer<DrawerType>();

    DrawerInfo<DrawerType> drawer_info = get_drawer<DrawerType>();

    return { drawer_info.id, drawer_info.pointer };
}
}