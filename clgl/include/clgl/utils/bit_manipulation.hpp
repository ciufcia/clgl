#pragma once

#include "clgl/typedefs.hpp"

#define _CLGL_CHECK_BIT(n, p) ((n) & (1 << p))
#define _CLGL_SET_BIT(n, p) ((n) | (1 << p))
#define _CLGL_UNSET_BIT(n, p) ((n) & (~(1 << p)))
#define _CLGL_FLIP_BIT(n, p) ((n) ^ (1 << p))

#define _CLGL_BIT_MASK(n, mask) ((n) & (mask))

namespace clgl::utils
{
[[nodiscard]] U8 reverse_U8_bits(U8 value);
}