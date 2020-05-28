#pragma once
#include "adlRange.h"

namespace array19 {

template<class C> struct WithIndex {
    using It = decltype(adlBegin(*static_cast<C*>(nullptr)));
    using Res = decltype(**static_cast<It*>(nullptr));

    struct result {
        Res value;
        size_t index;
    };

    struct iterator {
        constexpr iterator(It it) : it(it) {}

        [[nodiscard]] constexpr auto operator*() const -> result { return {*it, index}; }

        constexpr auto operator++() -> iterator& { return (++it, ++index, *this); }
        constexpr auto operator--() -> iterator& { return (--it, --index, *this); }

        constexpr bool operator==(const iterator& o) const { return it == o.it; }
        constexpr bool operator!=(const iterator& o) const { return !(*this == o); }

    private:
        It it;
        size_t index{};
    };

    constexpr WithIndex(C& c) : c(c) {}

    [[nodiscard]] constexpr auto begin() -> iterator { return {adlBegin(c)}; }
    [[nodiscard]] constexpr auto end() -> iterator { return {adlEnd(c)}; }

private:
    C& c;
};

template<class C> WithIndex(C&)->WithIndex<C>;

} // namespace array19
