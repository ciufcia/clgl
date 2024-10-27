#include "CLGL/utils/math.hpp"
#include <cmath>

clgl::F32 clgl::utils::round_halfway_toward_zero(F32 number) {
	if (number - std::floorf(number) == 0.5f) return std::truncf(number);
	else                                      return std::roundf(number);
}