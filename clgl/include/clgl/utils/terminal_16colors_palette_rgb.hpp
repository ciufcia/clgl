#pragma once

#include "clgl/color.hpp"

namespace clgl::utils::terminal_16colors_rgb
{
constexpr Color colors[16]
{
    { 0u, 0u, 0u },       // Black
    { 0u, 0u, 128u },     // Dark Blue
    { 0u, 128u, 0u },     // Dark Green
    { 0u, 128u, 128u },   // Dark Cyan
    { 128u, 0u, 0u },     // Dark Red
    { 128u, 0u, 128u },   // Dark Magenta
    { 128u, 128u, 0u },   // Dark Yellow
    { 192u, 192u, 192u }, // Light Gray
    { 128u, 128u, 128u }, // Gray
    { 0u, 0u, 255u },     // Blue
    { 0u, 255u, 0u },     // Green
    { 0u, 255u, 255u },   // Cyan
    { 255u, 0u, 0u },     // Red
    { 255u, 0u, 255u },   // Magenta
    { 255u, 255u, 0u },   // Yellow
    { 255u, 255u, 255u }  // White
};
}
