#pragma once
#include "Span.h"

namespace array19 {

/// view a single object as a span of one element
template<class T> static constexpr auto spanOne(T const& data) noexcept -> Span<T const> { return Span{&data, 1}; }

template<class T> static constexpr auto amendSpanOne(T& data) noexcept -> Span<T> { return Span{&data, 1}; }

template<class T>
static constexpr auto moveSpanOne(T&& data) noexcept -> Span<T&&> requires(std::is_rvalue_reference_v<T &&>) {
    return Span<T&&>{&data, 1};
}

} // namespace array19
