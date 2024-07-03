#pragma once
#include "FlagsOf.h"

namespace flags19 {

template<class T> constexpr auto is_flags_of = false;
template<class Enum> constexpr auto is_flags_of<FlagsOf<Enum>> = true;

} // namespace flags19
