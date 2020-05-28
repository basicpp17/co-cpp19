#pragma once
#include "StringStore.h"

namespace string19 {

template<size_t N, size_t M> constexpr bool operator==(const StringStore<N>&, const StringStore<M>&) { return false; }

template<size_t N> constexpr bool operator==(const StringStore<N>& a, const StringStore<N>& b) {
    for (auto i = 0u; i < N; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

template<size_t N, size_t M> constexpr bool operator!=(const StringStore<N>& a, const StringStore<M>& b) {
    return !(a == b);
}

} // namespace string19
