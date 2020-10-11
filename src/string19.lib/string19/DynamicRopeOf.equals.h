#pragma once
#include "DynamicRopeOf.h"
#include "array19/SliceOf.equals.h"
#include "variant19/Variant.equals.h"

namespace string19 {

template<class... Ts> auto operator==(const DynamicRopeOf<Ts...>& l, const DynamicRopeOf<Ts...>& r) noexcept -> bool {
    return l.slice() == r.slice();
}

template<class... Ts> auto operator!=(const DynamicRopeOf<Ts...>& l, const DynamicRopeOf<Ts...>& r) noexcept -> bool {
    return !(l == r);
}

} // namespace string19
