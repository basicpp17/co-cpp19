#pragma once
#include "flags19/FlagsOf.h" // requires variant19
#include "serialize.h"

namespace serialize19 {

template<Archive A, class Enum> void serialize(A& a, flags19::FlagsOf<Enum>& flags) {
    using Value = typename flags19::FlagsOf<Enum>::Value;
    auto value = static_cast<Value>(flags);
    serialize(a, value);
    if constexpr (A::mode == ArchiveMode::Read) {
        flags = flags19::FlagsOf<Enum>{value};
    }
}

} // namespace serialize19
