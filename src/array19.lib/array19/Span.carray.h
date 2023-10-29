#pragma once
#include "Span.h"

#include <stddef.h> // size_t
namespace array19 {

template<class T, size_t N> using CArray = T[N];

template<class T, size_t N> constexpr auto spanOfCArray(CArray<T const, N> const& array) noexcept -> Span<T const> {
    return Span<T const>{array, N};
}

template<class T, size_t N> constexpr auto amendSpanOfCArray(CArray<T, N>& array) noexcept -> Span<T> {
    return Span<T>{array, N};
}
template<class T, size_t N> constexpr auto moveSpanOfCArray(CArray<T, N>&& array) noexcept -> Span<T&&> {
    return Span<T&&>{array, N};
}

} // namespace array19
