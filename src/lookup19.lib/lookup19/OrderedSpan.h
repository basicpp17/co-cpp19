#pragma once
#include "array19/Span.h"

#include <stddef.h> // size_t

namespace lookup19 {

using array19::Span;

/// Readonly Span an ordered array
///
/// note:
/// * if we allow modifications ordering might be violated
template<class T, class L> struct OrderedSpan : Span<T> {
    using Data = typename Span<T>::Data;
    using Less = L;
    using Count = size_t;
    using Index = size_t;

public:
    constexpr OrderedSpan() = default;
    constexpr explicit OrderedSpan(Data* data, Count count) noexcept : Span<T>{data, count} {}
    explicit constexpr OrderedSpan(Span<T> span) noexcept : Span<T>{span} {}

    // requires: offset + count <= count()
    [[nodiscard]] constexpr auto slice(Index offset, Count count) const noexcept -> OrderedSpan {
        return OrderedSpan{this->begin() + offset, count};
    }
    // requires: offset <= count()
    [[nodiscard]] constexpr auto skip(Index offset) const noexcept -> OrderedSpan {
        return OrderedSpan{this->begin() + offset, static_cast<Count>(this->count() - offset)};
    }
    // requires: count <= count()
    [[nodiscard]] constexpr auto head(Count count) const noexcept -> OrderedSpan {
        return OrderedSpan{this->begin(), count}; //
    }
    // requires: count <= count()
    [[nodiscard]] constexpr auto tail(Count count) const noexcept -> OrderedSpan {
        return OrderedSpan{this->begin() + this->count() - count, count};
    }
    // requires: count <= count()
    [[nodiscard]] constexpr auto drop_tail(Count count) const noexcept -> OrderedSpan {
        return OrderedSpan{this->begin(), static_cast<Count>(this->count() - count)};
    }

    template<class Key> [[nodiscard]] constexpr auto lowerBound(Key&& key) -> Data* {
        auto less = Less{};
        auto lower_bound = this->begin();
        auto count = this->count();
        while (count > 0) {
            auto half = count >> 1;
            if (less(lower_bound[half], key)) {
                lower_bound += half + 1;
                count -= half + 1;
            }
            else {
                count = half;
            }
        }
        return lower_bound;
    }

    template<class Key> [[nodiscard]] constexpr auto upperBound(Key&& key) -> Data* {
        auto less = Less{};
        auto upper_bound = this->begin();
        auto count = this->count();
        while (count > 0) {
            auto half = count >> 1;
            if (!less(key, upper_bound[half])) {
                upper_bound += half + 1;
                count -= half + 1;
            }
            else {
                count = half;
            }
        }
        return upper_bound;
    }

    template<class Key> [[nodiscard]] constexpr auto equalRange(Key&& key) -> OrderedSpan {
        auto less = Less{};
        auto result = OrderedSpan{*this};
        while (result.count() > 0) {
            auto half = result.count() >> 1;
            if (less(result[half], key)) {
                result = result.skip(half + 1);
            }
            else if (less(key, result[half])) {
                result = result.head(half);
            }
            else {
                auto l = result.head(half).lowerBound(key);
                auto u = result.skip(half + 1).upperBound(key);
                return OrderedSpan{l, static_cast<Count>(u - l)};
            }
        }
        return result;
    }
};

template<class T, class Less> struct OrderedSpan<T&, Less>; // wont work - use OrderedSpan<T, Less>
template<class T, class Less> struct OrderedSpan<T const&&, Less>; // that's nonsense

template<class Less, class T> constexpr auto orderedSpan(T const* data, size_t count) -> OrderedSpan<T const, Less> {
    return OrderedSpan<T const, Less>{data, count};
}
template<class Less, class T> constexpr auto amendOrderedSpan(T* data, size_t count) -> OrderedSpan<T, Less> {
    return OrderedSpan<T, Less>{data, count};
}
template<class Less, class T> constexpr auto moveOrderedSpan(T* data, size_t count) -> OrderedSpan<T&&, Less> {
    return OrderedSpan<T&&, Less>{data, count};
}

} // namespace lookup19
