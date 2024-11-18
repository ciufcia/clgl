#include "clgl/clgl.hpp"
#include <iostream>
#include <unordered_map>

int main() {
    clgl::App app { { 320u, 200u }, { L"Consolas", { 4u, 4u } } };
    clgl::Event event;

    app.get_screen().enable_color_blending(false);

    clgl::ColorMappings *color_mappings = app.get_resource_manager().load_resource<clgl::ColorMappings>();
    color_mappings->load("clgl_resources/color_mappings.clcm");

    app.get_screen().register_and_set_drawer<clgl::drawers::Colors16Grayscale>();

    auto texture = clgl::Texture::create("thing.jpg", L'\x2588');

    clgl::Sprite point;
    point.set_texture(texture);

    point.set_scale({ 0.1f, 0.1f });

    /*
    clgl::Polygon point {5u, true, clgl::Pixel(clgl::Color(245,245,245, 255), L'\x2588')};

    point.set_point(0u, {0, 0});
    point.set_point(1u, {5, 2});
    point.set_point(2u, {17, 20});
    point.set_point(3u, {14, 33});
    point.set_point(4u, {-4, 10});

    clgl::Vec2F centroid = point.calculate_centroid();

    point.set_origin(centroid);

    point.set_scale({1, 1});

    point.move({ 40.f, 40.f });*/

    while (true) {
        while (app.get_input().pollEvent(event)) {
            if (event.type == clgl::EventType::Key) {
                if (event.event.keyEvent.keycode == clgl::KeyCode::P) {
                    if (event.event.keyEvent.pressed()) {
                        app.close();
                        exit(0);
                    }
                }
            }
        }

        clgl::Microseconds delta_time = app.get_last_frame_time<clgl::Microseconds>();
        std::chrono::duration<float> f_delta_time = delta_time;

        app.get_screen().set_title(std::to_wstring(1.f / f_delta_time.count()));

        if (app.get_input().is_key_pressed(clgl::KeyCode::A)) {
            point.move({-100.f * f_delta_time.count(), 0.f});
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::D)) {
            point.move({100.f * f_delta_time.count(), 0.f});
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::W)) {
            point.move({0.f, -100.f * f_delta_time.count()});
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::S)) {
            point.move({0.f, 100.f * f_delta_time.count()});
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::Q)) {
            point.rotate(-200.f * f_delta_time.count());
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::E)) {
            point.rotate(200.f * f_delta_time.count());
        }

        app.get_screen().fill({ { 0u, 0u, 0u }, L'#' });

        app.get_screen().draw_no_clipping(point);

        app.display();
    }
    return 0;
}