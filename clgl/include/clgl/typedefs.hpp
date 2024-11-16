#pragma once

#include <stdint.h>

namespace clgl
{
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t   I8;
typedef int16_t  I16;
typedef int32_t  I32;
typedef int64_t  I64;

typedef float    F32;
typedef double   F64;

typedef U32      CLGLResourceID;

// must be an unsigned integer
// the first bit of the type is reserved for overdraw
typedef U8       FlagBufferType;
}