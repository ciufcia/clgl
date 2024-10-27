#pragma once

#include "winapi_include.hpp"

namespace clgl::winapi
{
struct Handles
{
    HWND   window_handle;
    HANDLE input_handle;
    HANDLE output_handle;
};
}