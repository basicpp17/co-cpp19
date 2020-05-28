#pragma once
#include "PackedOptional.h"

namespace optional19 {

template<auto mI> constexpr auto operator==(const PackedOptional<mI>& a, const PackedOptional<mI>& b) {
    return (bool)a ? (bool)b && a.value() == b.value() : !(bool)b;
}
template<auto mI> constexpr auto operator!=(const PackedOptional<mI>& a, const PackedOptional<mI>& b) {
    return !(a == b);
}

} // namespace optional19
