#pragma once

#include "color.hpp"

namespace clgl
{
typedef wchar_t Character;

struct Pixel
{
    Pixel() = default;
    Pixel(Color color, Character character) : color(color), character(character) {}

    Color     color;
    Character character;
};
}