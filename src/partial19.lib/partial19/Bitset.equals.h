#pragma once
#include "Bitset.h"

namespace partial19 {

template<class T, size_t Len>
constexpr bool operator==(const BitsetArray<T, Len>& l, const BitsetArray<T, Len>& r) noexcept {
    for (auto i = 0u; i < Len; i++)
        if (l.m[i] != r.m[i]) return false;
    return true;
}

template<class T> constexpr bool operator==(const BitsetArray<T, 1>& l, const BitsetArray<T, 1>& r) noexcept {
    return l.m == r.m;
}

template<class T, size_t Len>
constexpr bool operator!=(const BitsetArray<T, Len>& l, const BitsetArray<T, Len>& r) noexcept {
    return !(l == r);
}

} // namespace partial19
