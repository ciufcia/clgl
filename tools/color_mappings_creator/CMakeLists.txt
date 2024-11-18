find_package(OpenMP REQUIRED)

add_executable(clgl_color_mappings_creator color_mappings_creator.cpp)

target_link_libraries(clgl_color_mappings_creator PUBLIC ${PROJECT_NAME} OpenMP::OpenMP_CXX)