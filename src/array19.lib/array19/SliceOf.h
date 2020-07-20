#pragma once
#include <cstddef> // size_t

namespace array19 {

/// View on a slice of consequtive memory
/// Usage:
///     SliceOf<int> // allow modification of values
///     SliceOf<const int> // values are treated const
///
/// Note: Slice itself is an unmodifiable value object
template<class T> struct SliceOf {
    using Element = T;
    using Count = size_t;
    using Index = size_t;

    constexpr SliceOf() = default;
    constexpr explicit SliceOf(Element* data, Count count) noexcept : m_data(data), m_count(count) {}
    constexpr explicit SliceOf(Element& data) noexcept : m_data(&data), m_count(1) {}

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] constexpr auto count() const noexcept -> Count { return m_count; }
    [[nodiscard]] constexpr auto slice(Index offset, Count count) const noexcept -> SliceOf {
        return SliceOf{m_data + offset, count};
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> Element* { return m_data; }
    [[nodiscard]] constexpr auto end() const noexcept -> Element* { return m_data + m_count; }
    [[nodiscard]] constexpr auto operator[](Index index) const noexcept -> Element& { return *(m_data + index); }

    [[nodiscard]] constexpr operator SliceOf<const T>() const noexcept { return SliceOf<const T>{m_data, m_count}; }

private:
    Element* m_data{};
    Count m_count{};
};

} // namespace array19
