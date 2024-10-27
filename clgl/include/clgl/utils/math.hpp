#pragma once

#include "clgl/typedefs.hpp"

namespace clgl
{
namespace utils
{
F32 round_halfway_toward_zero(F32 number);

constexpr F32 inverse_180 = 1.f / 180.f;
}
}