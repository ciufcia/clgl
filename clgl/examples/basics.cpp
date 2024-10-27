#include "clgl/clgl.hpp"
#include <iostream>

int main() {
    clgl::App app { { 100u, 40u }, { L"Consolas", { 10u, 10u } } };
    clgl::Event event;

    clgl::Polygon point {5u, true, clgl::Pixel(clgl::Color(180u, 64u, 255u), L'@')};

    point.set_point(0u, {0, 0});
    point.set_point(1u, {5, 2});
    point.set_point(2u, {17, 20});
    point.set_point(3u, {14, 33});
    point.set_point(4u, {-4, 10});

    point.move({10.f, 0.f});

    while (true) {
        while (app.input.pollEvent(event)) {
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

        if (app.input.is_key_pressed(clgl::KeyCode::A)) {
            point.move({-10.f * f_delta_time.count(), 0.f});
        }
        if (app.input.is_key_pressed(clgl::KeyCode::D)) {
            point.move({10.f * f_delta_time.count(), 0.f});
        }
        if (app.input.is_key_pressed(clgl::KeyCode::W)) {
            point.move({0.f, -10.f * f_delta_time.count()});
        }
        if (app.input.is_key_pressed(clgl::KeyCode::S)) {
            point.move({0.f, 10.f * f_delta_time.count()});
        }
        if (app.input.is_key_pressed(clgl::KeyCode::Q)) {
            point.rotate(-30.f * f_delta_time.count());
        }
        if (app.input.is_key_pressed(clgl::KeyCode::E)) {
            point.rotate(30.f * f_delta_time.count());
        }

        app.screen.fill({ { 0u, 0u, 0u }, L'#' });

        app.screen.draw(point);

        app.display();
    }

    return 0;
}