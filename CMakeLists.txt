cmake_minimum_required(VERSION 3.26)

project(
    clgl
    VERSION 0.1.0
    DESCRIPTION "command line graphics library"
    LANGUAGES CXX
)

if(NOT WIN32)
    message(FATAL_ERROR "clgl only works on windows")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

option(BUILD_SHARED_LIBS OFF)

option(clgl_BUILD_TOOLS ON)
option(clgl_BUILD_EXAMPLES ON)

add_subdirectory(clgl)

add_subdirectory(tools)