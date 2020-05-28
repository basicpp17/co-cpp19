#pragma once
#include <stddef.h> // size_t

namespace array19 {

/// simplified version of std::array
/// * no member types
/// * no `operator[]` - use `at` and `amendAt`
/// * no exceptions
template<class T, size_t C> struct Array {
    using Element = T;
    static constexpr auto count = C;
    using Size = decltype(C);
    using Index = size_t;

    T m[count];

    [[nodiscard]] constexpr auto begin() const noexcept -> const T* { return m; }
    [[nodiscard]] constexpr auto end() const noexcept -> const T* { return m + C; }
    [[nodiscard]] constexpr auto at(size_t i) const noexcept -> const T& { return m[i]; }

    // hint: use `amendSliceOfArray()` if you need to iterate and mutate
    [[nodiscard]] constexpr auto amendBegin() noexcept -> T* { return m; }
    [[nodiscard]] constexpr auto amendEnd() noexcept -> T* { return m + C; }
    [[nodiscard]] constexpr auto amendAt(size_t i) noexcept -> T& { return m[i]; }
};

/// deduction guide without checking all types are the same
/// usage:
///     Array{1, 2, 3};
template<class T, class... Ts> Array(T, Ts...) -> Array<T, 1 + sizeof...(Ts)>;

} // namespace array19
