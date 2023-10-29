#pragma once
#include "Span.h"

#include <stddef.h> // size_t

namespace array19 {

/// simplified version of std::array
/// * no member types
/// * no exceptions
template<class T, size_t N> struct Array {
    using Element = T;
    static constexpr auto count = N;

    T m[count];

    bool operator==(const Array&) const = default;

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return false; }

    [[nodiscard]] constexpr auto begin() const noexcept -> T const* { return m; }
    [[nodiscard]] constexpr auto end() const noexcept -> T const* { return m + count; }

    // requires: index < count
    [[nodiscard]] constexpr auto operator[](size_t index) const noexcept -> T const& { return m[index]; }

    [[nodiscard]] constexpr operator Span<T const>() const& noexcept { return Span<T const>{m, count}; }
    [[nodiscard]] constexpr auto amend() noexcept -> Span<T> { return Span<T>{m, count}; }
    [[nodiscard]] constexpr auto move() noexcept -> Span<T&&> { return Span<T&&>{m, count}; }
};

template<class T> struct Array<T, 0u> {
    using Element = T;
    static constexpr auto count = size_t{};

    bool operator==(const Array&) const = default;

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return true; }

    [[nodiscard]] constexpr auto begin() const noexcept -> T const* { return nullptr; }
    [[nodiscard]] constexpr auto end() const noexcept -> T const* { return nullptr; }

    [[nodiscard]] constexpr operator Span<T const>() const& noexcept { return Span<T const>{nullptr, count}; }
    [[nodiscard]] constexpr auto amend() noexcept -> Span<T> { return Span<T>{nullptr, count}; }
    [[nodiscard]] constexpr auto move() noexcept -> Span<T&&> { return Span<T&&>{nullptr, count}; }
};

/// simplified deduction guide
/// * not checking that all types are the same
/// usage:
///     Array{1, 2, 3};
template<class T, class... Ts> Array(T, Ts...) -> Array<T, 1 + sizeof...(Ts)>;

} // namespace array19
