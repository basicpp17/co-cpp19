#pragma once
#include "Array.h"
#include "Span.h"

#include <stddef.h> // size_t
#include <utility> // std::index_sequence

namespace array19 {

/// note:
///     You have to know the size at compile time to create an Array!
/// usage:
///     constexpr auto span = Span{...};
///     constexpr auto array = storeSpan<span.count()>(span);
template<size_t N, class T, class Data = typename Span<T>::Data>
constexpr auto storeSpan(Span<T> const& span) noexcept -> Array<Data, N> {
    // note: using index_sequence allows constructing arrays of const elements, also takes care of move
    return [&]<size_t... Is>(std::index_sequence<Is...>*)->Array<Data, N> { return {span[Is]...}; }
    (static_cast<std::make_index_sequence<N>*>(nullptr));
}

} // namespace array19
