#include "clgl/clgl.hpp"

int main() {
    // create an app in a window of width and height of 50 characters
    // set the console font to Consolas and set the character size to 10 by 10 pixels
    clgl::App app { clgl::Vec2U(50u, 50u), { L"Consolas", clgl::Vec2U(10u, 10u) } };
    // set maximum fps
    app.set_maximum_fps(60u);

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
            }
        }

        // fill the screen with white @
        app.get_screen().fill(clgl::Pixel(clgl::Color(255u, 255u, 255u), L'@'));

        // update the app
        app.display();
    }

    return 0;
}