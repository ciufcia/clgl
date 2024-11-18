#pragma once

#include "typedefs.hpp"
#include <string>
#include <numbers>

namespace clgl::constants
{
constexpr U32            color_palette_size          = 16;
constexpr U32            color_space_size_24bit      = 16'777'216;
constexpr U32            clcm_size                   = color_space_size_24bit + color_palette_size;

const     std::wstring   ESC                         = L"\x1b";
const     std::wstring   ESCB                        = ESC + L"[";
const     std::wstring   ESCC                        = ESCB + L"38;2;";
const     std::wstring   ESCE                        = L"m";
const     std::wstring   ANSI_NEWLINE                = ESCB + L"1E" + ESCB + L"0G";
const     U32            ANSI_color_length           = ESCC.size() + 11u + ESCE.size();
const     U32            ANSI_coloring_output_length = ESCC.size() + 12u + ESCE.size();

constexpr F32            half_pi                     = std::numbers::pi / 2.f;

constexpr FlagBufferType overdraw_flag               = 1u;
}