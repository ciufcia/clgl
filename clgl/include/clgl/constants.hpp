#pragma once

#include "typedefs.hpp"
#include <string>

namespace clgl::constants
{
constexpr U32          color_space_size_24bit      = 16'777'216;
const     std::string  resources_folder_path       = "clgl_resources/";

const     std::wstring ESC                         = L"\x1b";
const     std::wstring ESCB                        = ESC + L"[";
const     std::wstring ESCC                        = ESCB + L"38;2;";
const     std::wstring ESCE                        = L"m";
const     std::wstring ANSI_NEWLINE                = constants::ESCB + L"1E" + constants::ESCB + L"0G";
const     U32          ANSI_color_length           = ESCC.size() + 11u + ESCE.size();
const     U32          ANSI_coloring_output_length = ESCC.size() + 12u + ESCE.size();
}