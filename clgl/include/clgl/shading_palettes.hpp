#pragma once

#include <string>

// each shading palette must consist of 16 characters
namespace clgl::shading_palettes
{
const std::wstring default_palette_16 = L" .:^-=+|v0*#&%@$";
const std::wstring box_palette_16     = L"   \x2591\x2591\x2591\x2592\x2592\x2592\x2593\x2593\x2593\x2588\x2588\x2588\x2588";
const std::wstring default_palette_4  = L".=*@";
const std::wstring box_palette_4      = L"\x2591\x2592\x2593\x2588";
}