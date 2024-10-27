#pragma once

#include "winapi_include.hpp"

namespace clgl::utils::terminal_16_colors
{
enum class ForegroundColor
{
    Black       = 0u,
    DarkBlue    = FOREGROUND_BLUE,
    DarkGreen   = FOREGROUND_GREEN,
    DarkCyan    =   DarkGreen | DarkBlue,
    DarkRed     = FOREGROUND_RED,
    DarkMagenta =     DarkRed | DarkBlue,
    DarkYellow  =     DarkRed | DarkGreen,
    LightGray   =     DarkRed | DarkGreen | DarkBlue,

    Gray        =       Black | FOREGROUND_INTENSITY,
    Blue        =    DarkBlue | FOREGROUND_INTENSITY,
    Green       =   DarkGreen | FOREGROUND_INTENSITY,
    Cyan        =    DarkCyan | FOREGROUND_INTENSITY,
    Red         =     DarkRed | FOREGROUND_INTENSITY,
    Magenta     = DarkMagenta | FOREGROUND_INTENSITY,
    Yellow      =  DarkYellow | FOREGROUND_INTENSITY,
    White       =   LightGray | FOREGROUND_INTENSITY
};

enum class BackgroundColor
{
    Black       = 0u,
    DarkRed     = BACKGROUND_RED,
    DarkGreen   = BACKGROUND_GREEN,
    DarkBlue    = BACKGROUND_BLUE,
    DarkYellow  =       DarkRed | DarkGreen,
    DarkMagenta =       DarkRed | DarkBlue,
    DarkCyan    =     DarkGreen | DarkBlue,
    LightGray   =       DarkRed | DarkGreen | DarkBlue,

    Gray        =         Black | BACKGROUND_INTENSITY,
    Red         =       DarkRed | BACKGROUND_INTENSITY,
    Green       =     DarkGreen | BACKGROUND_INTENSITY,
    Blue        =      DarkBlue | BACKGROUND_INTENSITY,
    Yellow      =    DarkYellow | BACKGROUND_INTENSITY,
    Magenta     =   DarkMagenta | BACKGROUND_INTENSITY,
    Cyan        =      DarkCyan | BACKGROUND_INTENSITY,
    White       =     LightGray | BACKGROUND_INTENSITY
};
}