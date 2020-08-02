#pragma once

namespace lookup19 {

template<class T, class Less> struct SortedSlice {
    using Element = T;
    using Count = size_t;
    using Index = size_t;

    constexpr SortedSlice() = default;
    constexpr explicit SortedSlice(Element* data, Element* end) noexcept : m_data(data), m_count(end - data) {}
    constexpr explicit SortedSlice(Element* data, Count count) noexcept : m_data(data), m_count(count) {}

    [[nodiscard]] constexpr auto count() const -> Count { return m_count; }
    [[nodiscard]] constexpr auto begin() const& noexcept -> Element* { return m_data; }
    [[nodiscard]] constexpr auto end() const& -> Element* { return m_data + m_count; }

    [[nodiscard]] constexpr operator SortedSlice<const T>() const noexcept {
        return SortedSlice<const T>{m_data, m_count};
    }

    template<class K>[[nodiscard]] constexpr auto lowerBound(K&& key) -> Element* {
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

    template<class K>[[nodiscard]] constexpr auto upperBound(K&& key) -> Element* {
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

    template<class K>[[nodiscard]] constexpr auto equalRange(K&& key) -> SortedSlice {
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
                auto l = SortedSlice{lower_bound, lower_bound + half}.lowerBound(key);
                auto u = SortedSlice{lower_bound + half + 1, upper_bound}.upperBound(key);
                return SortedSlice{l, u};
            }
        }
        return SortedSlice{lower_bound, upper_bound};
    }

private:
    Element* m_data{};
    Count m_count{};
};

} // namespace lookup19
