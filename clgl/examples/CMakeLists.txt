add_executable(basics basics.cpp)
target_link_libraries(basics PUBLIC ${PROJECT_NAME})

add_executable(drawables drawables.cpp)
target_link_libraries(drawables PUBLIC ${PROJECT_NAME})

add_executable(drawers drawers.cpp)
target_link_libraries(drawers PUBLIC ${PROJECT_NAME})