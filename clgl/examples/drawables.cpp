#include "clgl/clgl.hpp"

int main() {
    // create an app in a window of width and height of 50 character
    // set the console font to Consolas and set the character size to 10 by 10 pixels
    clgl::App app { clgl::Vec2U(40u, 20u), { L"Consolas", clgl::Vec2U(20u, 20u) } };

    // set maximum fps
    app.set_maximum_fps(60u);

    // create a purple point represented at (15.f, 8.f)
    clgl::Point point { clgl::Vec2F(15.f, 8.f), clgl::Pixel(clgl::Color(255u, 0u, 255u, 128u), L'\x2588') };

    // create a blue line with its ends at (0.f, 20.f) and (40.f, 0.f) 
    clgl::Line line { clgl::Vec2F(0.f, 20.f), clgl::Vec2F(40.f, 0.f), clgl::Pixel(clgl::Color(0u, 0u, 255u, 96u), L'\x2588') };

    // create a white circle with radius of 10.f and a center at (10.f, 10.f) 
    clgl::Circle circle { clgl::Vec2F(10.f, 10.f), 10.f, true, clgl::Pixel(clgl::Color(0u, 255u, 255u, 255u), L'\x2588') };

    // create a yellow, filled polygon with 5 vertices
    clgl::Polygon polygon { 10u, true, clgl::Pixel(clgl::Color(255u, 255u, 0u, 200u), L'\x2588') };
    // set the vertices of the polygon
    polygon.set_point(0u, clgl::Vec2F(0.f, -10.f));
    polygon.set_point(1u, clgl::Vec2F(3.804f, -3.09f));
    polygon.set_point(2u, clgl::Vec2F(9.51f, -3.09f));
    polygon.set_point(3u, clgl::Vec2F(5.876f, 3.236f));
    polygon.set_point(4u, clgl::Vec2F(5.877f, 8.09f));
    polygon.set_point(5u, clgl::Vec2F(0.f, 4.f));
    polygon.set_point(6u, clgl::Vec2F(-5.877f, 8.09f));
    polygon.set_point(7u, clgl::Vec2F(-5.876f, 3.236f));
    polygon.set_point(8u, clgl::Vec2F(-9.51f, -3.09f));
    polygon.set_point(9u, clgl::Vec2F(-3.804f, -3.09f));

    // set the transformation origin of the polygon to its center
    polygon.set_origin(polygon.calculate_centroid());

    polygon.set_position(clgl::Vec2F(10.f, 10.f));

    float delta_time_in_seconds = 0.f;

    clgl::Vec2F point_direction { 1.f, 1.f };
    float point_speed = 20.f;

    float line_direction = 1.f;
    float line_speed = 10.f;

    float circle_direction = 1.f;
    float circle_speed = 25.f;

    float polygon_direction = 1.f;
    float polygon_speed = 15.f;
    float polygon_rotation_speed = 150.f;

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

                if (event.event.keyEvent.pressed() && event.event.keyEvent.keycode == clgl::KeyCode::C) {
                    app.get_screen().enable_color_blending(!app.get_screen().is_color_blending_enabled());
                }
            }
        }

        if (point.position.x <= 0.f) point_direction.x = 1.f;
        if (point.position.y <= 0.f) point_direction.y = 1.f; 
        if (point.position.x >= 40.f) point_direction.x = -1.f; 
        if (point.position.y >= 20.f) point_direction.y = -1.f; 

        if (line.end1.y >= 20.f) line_direction = -1.f;
        if (line.end1.y <= 0.f) line_direction = 1.f;

        if (circle.center.x + circle.radius >= 40.f) circle_direction = -1.f;
        if (circle.center.x - circle.radius <= 0.f) circle_direction = 1.f;

        if (polygon.get_position().x >= 30.f) polygon_direction = -1.f;
        if (polygon.get_position().x <= 10.f) polygon_direction = 1.f;

        point.move(point_direction * point_speed * delta_time_in_seconds);

        line.end1.y += line_direction * line_speed * delta_time_in_seconds;
        line.end2.y += line_direction * line_speed * delta_time_in_seconds * -1.f;

        circle.center.x += circle_direction * circle_speed * delta_time_in_seconds;

        polygon.move(clgl::Vec2F(polygon_speed * polygon_direction, 0.f) * delta_time_in_seconds);
        polygon.rotate(polygon_rotation_speed * delta_time_in_seconds);

        // fill the screen with black " "
        app.get_screen().fill(clgl::Pixel(clgl::Color(0u, 0u, 0u), L' '));

        // draw all the drawables
        app.get_screen().draw(circle);
        app.get_screen().draw(polygon);
        app.get_screen().draw(line);
        app.get_screen().draw(point);

        // update the app
        app.display();

        clgl::Microseconds delta_time = app.get_last_frame_time<clgl::Microseconds>();
        delta_time_in_seconds = std::chrono::duration<float>(delta_time).count();
    }

    return 0;
}