#include "clgl/clgl.hpp"
#include <iostream>
#include <unordered_map>

int main() {
    //clgl::Color color1 {100, 255, 255, 100};
    //clgl::Color color2 {0, 0, 0, 60};

    //std::cout << int(clgl::utils::blend_colors(color1, color2).r) << ", "<< int(clgl::utils::blend_colors(color1, color2).g) << ", "<< int(clgl::utils::blend_colors(color1, color2).b) << ", " << int(clgl::utils::blend_colors(color1, color2).a) << "\n";
 
    clgl::App app { { 100u, 100u }, { L"Consolas", { 5u, 5u } } };
    clgl::Event event;

    app.get_screen().enable_color_blending(true);

    /*
    clgl::Sprite point;
    auto p_texture = clgl::Texture::create("thing.png", L'\x2588');
    point.set_texture(p_texture);*/
    
    clgl::Polygon point {5u, true, clgl::Pixel(clgl::Color(0x3307ff, 127), L'\x2588')};

    point.set_point(0u, {0, 0});
    point.set_point(1u, {5, 2});
    point.set_point(2u, {17, 20});
    point.set_point(3u, {14, 33});
    point.set_point(4u, {-4, 10});

    clgl::Vec2F centroid = point.calculate_centroid();

    point.set_origin(centroid);

    point.set_scale({2, 2});

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
            point.move({-10.f * f_delta_time.count(), 0.f});
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::D)) {
            point.move({10.f * f_delta_time.count(), 0.f});
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::W)) {
            point.move({0.f, -10.f * f_delta_time.count()});
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::S)) {
            point.move({0.f, 10.f * f_delta_time.count()});
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::Q)) {
            point.rotate(-200.f * f_delta_time.count());
        }
        if (app.get_input().is_key_pressed(clgl::KeyCode::E)) {
            point.rotate(200.f * f_delta_time.count());
        }

        app.get_screen().fill({ { 0u, 0u, 0u }, L'#' });

        app.get_screen().draw(point);

        app.display();
    }

    return 0;
}