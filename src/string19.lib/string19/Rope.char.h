#pragma once
#include "ADL.h"

namespace string19 {

constexpr auto ropeCount(ADL*, char) -> size_t { return 1; }

constexpr auto ropeAppend(ADL*, char*& data, char c) { *data++ = c; }

} // namespace string19
