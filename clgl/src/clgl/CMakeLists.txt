find_package(OpenMP REQUIRED)

add_library(
    ${PROJECT_NAME}
    ../external/stb_image.cpp
    clgl.cpp
    color.cpp
    screen_buffer.cpp
    screen.cpp
    screen_writer.cpp
    drawer.cpp
    timings.cpp
    events.cpp
    input.cpp
    app.cpp
    texture.cpp
    transformable.cpp
    drawables/point.cpp
    drawables/line.cpp
    drawables/polygon.cpp
    drawables/circle.cpp
    drawables/intline.cpp
    drawables/sprite.cpp
    drawers/full_rgb.cpp
    drawers/colors_16.cpp
    drawers/colors_16_grayscale.cpp
    drawers/single_color_shaded.cpp
    drawers/single_color_16_shaded.cpp
    drawers/colors_16_grayscale_shaded.cpp
    utils/bit_manipulation.cpp
    utils/math.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC "../../include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)