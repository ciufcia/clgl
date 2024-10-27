#include "clgl/utils/bit_manipulation.hpp"

clgl::U8 clgl::utils::reverse_U8_bits(U8 value) {
    value = (value & 0xF0) >> 4 | (value & 0x0F) << 4;
    value = (value & 0xCC) >> 2 | (value & 0x33) << 2;
    value = (value & 0xAA) >> 1 | (value & 0x55) << 1;

    return value;
}