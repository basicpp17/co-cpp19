#pragma once
#include "ADL.h"
#include "Rope.Rope.h"
#include "StringStore.h"

namespace string19 {

template<class T> constexpr auto ropeLengthOf(const T& v) -> size_t { return ropeLengthOf(adl, v); }

/// @returns a StringStore that concats all parts of the Rope
/// usage:
///    constexpr auto rope = Rope{};
///    constexpr auto N = ropeLengthOf(rope);
///    constexpr auto store = ropeStore<N>(rope);
template<size_t N, class... Ts> constexpr auto ropeStore(const Rope<Ts...>& r) -> StringStore<N> {
    auto result = StringStore<N>{};
    auto* data = result.data;
    ropeAppend(adl, data, r);
    return result;
}

} // namespace string19
