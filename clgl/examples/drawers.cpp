#include "clgl/clgl.hpp"
#include <vector>

// make sure that you have the required files (judith.jpg, color_mappings.clcm) in the same directory as the executable

int main() {
    // create an app in a window of width and height of 50 character
    // set the console font to Consolas and set the character size to 10 by 10 pixels
    clgl::App app { clgl::Vec2U(98u, 200u), { L"Consolas", clgl::Vec2U(5u, 5u) } };

    // set maximum fps
    app.set_maximum_fps(60u);

    app.get_screen().set_title(L"Press C to change drawer");

    // create a texture and load the pixel data from a jpeg
    std::shared_ptr<clgl::Texture> texture = clgl::Texture::create("judith.jpg", L'\x2588');
    // create a sprite and assign our texture to it
    clgl::Sprite sprite;
    sprite.set_texture(texture);

    sprite.set_scale({0.5f, 0.5f});

    // create a ColorMappings resource that our drawers will be able to use
    clgl::ColorMappings *color_mappings = app.get_resource_manager().load_resource<clgl::ColorMappings>();
    // load the color mappings from a file
    color_mappings->load("color_mappings.clcm");

    // register some drawers and save their IDs
    std::vector<clgl::U32> drawer_ids;
    drawer_ids.push_back(0u); // id 0 is reserved for the "default" FullRGB drawer
    drawer_ids.push_back(app.get_screen().register_drawer<clgl::drawers::Colors16>().id);
    drawer_ids.push_back(app.get_screen().register_drawer<clgl::drawers::Colors16Grayscale>().id);
    // some drawer may require to be configured
    clgl::DrawerInfo c16gs_info = app.get_screen().register_drawer<clgl::drawers::Colors16GrayscaleShaded>();
    c16gs_info.pointer->set_shading_palette(clgl::shading_palettes::box_palette_4);
    drawer_ids.push_back(c16gs_info.id);

    clgl::DrawerInfo scs_info = app.get_screen().register_drawer<clgl::drawers::SingleColorShaded>();
    scs_info.pointer->set_shading_palette(clgl::shading_palettes::default_palette_16);
    scs_info.pointer->color = {0u, 255u, 0u};
    drawer_ids.push_back(scs_info.id);

    int current_drawer_index = 0;

    // main app loop
    while (true) {
        // create an event object, to which we'll be saving the data about a clgl Event
        clgl::Event event;
        // go through each one of the events that have accumulated during the last frame
        while (app.get_input().poll_event(event)) {
            // check if the event is related to the keyboard
            if (event.type == clgl::EventType::Key) {
                // check if a key was pressed and if the pressed key was P
                if (event.event.keyEvent.pressed() && event.event.keyEvent.keycode == clgl::KeyCode::P) {
                    // revert the console window to its original setting (if possible)
                    app.close();
                    exit(0);
                }

                // if C is pressed, switch to the next drawer
                if (event.event.keyEvent.pressed() && event.event.keyEvent.keycode == clgl::KeyCode::C) {
                    current_drawer_index = (current_drawer_index + 1) % drawer_ids.size();
                    app.get_screen().set_drawer(drawer_ids[current_drawer_index]);
                }
            }
        }

        // fill the screen with black " "
        app.get_screen().fill(clgl::Pixel(clgl::Color(0u, 0u, 0u), L' '));

        app.get_screen().draw(sprite);

        // update the app
        app.display();
    }

    return 0;
}