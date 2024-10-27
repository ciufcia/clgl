#pragma once

#include "vec.hpp"
#include <string>

namespace clgl
{
struct FontData
{
    FontData() = default;
    FontData(const std::wstring &face_name, const Vec2U &size, U32 weight = 0u, U32 family = 0u) : face_name(face_name), size(size), weight(weight), family(family) {}

    std::wstring face_name = L"Consolas";
    Vec2U        size      { 0u, 0u };
    U32          weight    = 0u;

    /**
     *  See 'tmPitchAndFamily' at https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-textmetrica
     */
    U32          family    = 0u;
};
}