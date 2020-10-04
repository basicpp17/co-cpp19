#pragma once
#include "ADL.h"

namespace string19 {

constexpr auto ropeLengthOf(ADL*, char) -> size_t { return 1; }

constexpr void ropeAppend(ADL*, char*& data, char c) { *data++ = c; }

} // namespace string19
