#pragma once
#include "Array.h"
#include "SliceOf.h"

namespace array19 {

template<class T, size_t N> constexpr auto sliceOfArray(const Array<T, N>& array) noexcept -> SliceOf<const T> {
    return SliceOf<const T>{array.m, N};
}

template<class T, size_t N> constexpr auto amendSliceOfArray(Array<T, N>& array) noexcept -> SliceOf<T> {
    return SliceOf<T>{array.m, N};
}

} // namespace array19