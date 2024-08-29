#pragma once
#include "array19/SliceOf.h"

#include <stddef.h> // size_t

namespace lookup19 {

using array19::SliceOf;

/// Readonly SliceOf an ordered array
///
/// note:
/// * if we allow modifications ordering might be violated
template<class T, class Less> struct OrderedSliceOf {
    using Element = const T;
    using Count = size_t;
    using Index = size_t;
    using Unordered = SliceOf<Element>;

private:
    Element* m_data{};
    Count m_count{};

public:
    constexpr OrderedSliceOf() = default;
    constexpr explicit OrderedSliceOf(Unordered slice) : m_data{slice.begin()}, m_count{slice.count()} {}
    constexpr explicit OrderedSliceOf(Element* data, Element* end)
            : m_data{data}
            , m_count{static_cast<size_t>(end - data)} {}
    constexpr explicit OrderedSliceOf(Element* data, Count count) : m_data{data}, m_count{count} {}

    [[nodiscard]] constexpr auto isEmpty() const -> bool { return m_count == 0; }
    [[nodiscard]] constexpr auto count() const -> Count { return m_count; }
    [[nodiscard]] constexpr auto begin() const& -> Element* { return m_data; }
    [[nodiscard]] constexpr auto end() const& -> Element* { return m_data + m_count; }
    [[nodiscard]] constexpr auto operator[](Index index) const -> Element& { return m_data[index]; }

    [[nodiscard]] constexpr operator Unordered() const { return Unordered{m_data, m_count}; }

    template<class K> [[nodiscard]] constexpr bool has(K&& key) const {
        auto it = lowerBound((K&&)key);
        return it != end() && *it == key;
    }

    template<class K> [[nodiscard]] constexpr auto lowerBound(K&& key) const -> Element* {
        auto less = Less{};
        auto lower_bound = m_data;
        auto count = m_count;
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

    template<class K> [[nodiscard]] constexpr auto upperBound(K&& key) const -> Element* {
        auto less = Less{};
        auto upper_bound = m_data;
        auto count = m_count;
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

    template<class K> [[nodiscard]] constexpr auto equalRange(K&& key) const -> OrderedSliceOf {
        auto less = Less{};
        auto count = m_count;
        auto lower_bound = m_data;
        auto upper_bound = m_data + count;
        while (count > 0) {
            auto half = count >> 1;
            if (less(lower_bound[half], key)) {
                lower_bound += half + 1;
                count -= half + 1;
            }
            else if (less(key, lower_bound[half])) {
                upper_bound = lower_bound + half;
                count = half;
            }
            else {
                auto l = OrderedSliceOf{lower_bound, lower_bound + half}.lowerBound(key);
                auto u = OrderedSliceOf{lower_bound + half + 1, upper_bound}.upperBound(key);
                return OrderedSliceOf{l, u};
            }
        }
        return OrderedSliceOf{lower_bound, upper_bound};
    }
};

template<class T, class Less> struct OrderedSliceOf<T&&, Less>; // use MoveSliceOf<T> for that

} // namespace lookup19
