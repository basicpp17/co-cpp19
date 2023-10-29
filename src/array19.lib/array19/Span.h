#pragma once
#include <stddef.h> // size_t
#include <type_traits> // std::remove_reference_t
#include <utility> // std::move

namespace array19 {

/// View on a span of consequtive memory
/// Usage:
///     Span<int> // allow modification of values
///     Span<int const> // values are treated const
///     Span<int&&> // mark for move
///
/// Note: Span itself is an unmodifiable value object
template<class T> struct Span {
    using Data = std::remove_reference_t<T>;
    using Count = size_t;
    using Index = size_t;
    static constexpr auto is_move = std::is_rvalue_reference_v<T>;

private:
    Data* m_data{};
    Count m_count{};

public:
    constexpr Span() = default;
    constexpr explicit Span(Data* data, Count count) noexcept : m_data{data}, m_count{count} {}

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] constexpr auto count() const noexcept -> Count { return m_count; }

    [[nodiscard]] constexpr auto begin() const noexcept -> Data* { return m_data; }
    [[nodiscard]] constexpr auto end() const noexcept -> Data* { return m_data + m_count; }
    // requires: index < m_count
    [[nodiscard]] constexpr auto operator[](Index index) const noexcept -> Data& requires(!is_move) {
        return *(m_data + index);
    }
    [[nodiscard]] constexpr auto operator[](Index index) const noexcept -> Data requires(is_move) {
        return std::move(*(m_data + index));
    }

    // allow use of mutable/move span in places where const is expected
    [[nodiscard]] constexpr operator Span<Data const>() const noexcept { return Span<Data const>{m_data, m_count}; }

    // requires: offset + count <= m_count
    [[nodiscard]] constexpr auto slice(Index offset, Count count) const noexcept -> Span {
        return Span{m_data + offset, count};
    }
    // requires: offset <= m_count
    [[nodiscard]] constexpr auto skip(Index offset) const noexcept -> Span {
        return Span{m_data + offset, static_cast<Count>(m_count - offset)};
    }
    // requires: count <= m_count
    [[nodiscard]] constexpr auto head(Count count) const noexcept -> Span {
        return Span{m_data, count}; //
    }
    // requires: count <= m_count
    [[nodiscard]] constexpr auto tail(Count count) const noexcept -> Span {
        return Span{m_data + m_count - count, count};
    }
    // requires: count <= m_count
    [[nodiscard]] constexpr auto drop_tail(Count count) const noexcept -> Span {
        return Span{m_data, static_cast<Count>(m_count - count)};
    }
};

template<class T> struct Span<T&>; // wont work - use Span<T>
template<class T> struct Span<T const&&>; // that's nonsense

template<class T> Span(T*, size_t) -> Span<T>;

template<class T> constexpr auto moveSpan(T* data, size_t count) -> Span<T&&> { return Span<T&&>{data, count}; }

} // namespace array19
