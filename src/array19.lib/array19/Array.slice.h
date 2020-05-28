#pragma once
#include "Array.h"
#include "SliceOf.h"

#include <utility>

namespace array19 {

template<class T, size_t... Is>
constexpr auto arrayFrom(const SliceOf<T>& slice, std::index_sequence<Is...>*) noexcept -> Array<T, sizeof...(Is)> {
    return {slice.at(Is)...};
}

template<class T, size_t... Is>
constexpr auto arrayFrom(const SliceOf<const T>& slice, std::index_sequence<Is...>*) noexcept
    -> Array<T, sizeof...(Is)> {
    return {slice.at(Is)...};
}

} // namespace array19
