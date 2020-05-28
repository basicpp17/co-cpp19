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
    constexpr SliceOf(const SliceOf&) = default;
    constexpr SliceOf& operator=(const SliceOf&) = default;
    constexpr SliceOf(SliceOf&&) = default;
    constexpr SliceOf& operator=(SliceOf&&) = default;

    constexpr explicit SliceOf(Element* data, Element* end) noexcept : m_data(data), m_count(end - data) {}
    constexpr explicit SliceOf(Element* data, Count count) noexcept : m_data(data), m_count(count) {}

    [[nodiscard]] constexpr auto count() const -> Count { return m_count; }
    [[nodiscard]] constexpr auto slice(Index offset, Count count) const -> SliceOf {
        return SliceOf{m_data + offset, count};
    }

    [[nodiscard]] constexpr auto begin() const& noexcept -> Element* { return m_data; }
    [[nodiscard]] constexpr auto end() const& -> Element* { return m_data + m_count; }
    [[nodiscard]] constexpr auto at(Index index) const& noexcept -> Element& { return *(m_data + index); }

    [[nodiscard]] constexpr auto asConst() const noexcept -> SliceOf<const T> {
        return SliceOf<const T>{m_data, m_count};
    }

private:
    Element* m_data{};
    Count m_count{};
};

} // namespace array19
