#pragma once
#include "ADL.h"
#include "StringStore.h"

namespace string19 {

template<size_t N> constexpr auto ropeLengthOf(ADL*, const StringStore<N>&) -> size_t { return N; }

template<size_t N> constexpr void ropeAppend(ADL*, char*& data, const StringStore<N>& s) {
    for (auto i = 0u; i < N; ++i) *data++ = s[i];
}

} // namespace string19
