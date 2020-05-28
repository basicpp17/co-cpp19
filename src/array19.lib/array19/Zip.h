#pragma once
#include "adlRange.h"

namespace array19 {

/// Zip allows to iterate two containers in lockstep
///
/// usage:
///   for (auto [valueA, valueB] : Zip{a, b});
template<class A, class B> struct Zip {
    using AIt = decltype(adlBegin(*static_cast<A*>(nullptr)));
    using BIt = decltype(adlBegin(*static_cast<B*>(nullptr)));
    using ARes = decltype(**static_cast<AIt*>(nullptr));
    using BRes = decltype(**static_cast<BIt*>(nullptr));

    struct result {
        ARes a;
        BRes b;
    };

    struct iterator {
        constexpr iterator(AIt aIt, BIt bIt) : aIt(aIt), bIt(bIt) {}

        constexpr auto operator*() const -> result { return {*aIt, *bIt}; }

        constexpr auto operator++() -> iterator& { return (++aIt, ++bIt, *this); }
        constexpr auto operator--() -> iterator& { return (--aIt, --bIt, *this); }

        constexpr bool operator==(const iterator& o) const { return aIt == o.aIt && bIt == o.bIt; }
        constexpr bool operator!=(const iterator& o) const { return !(*this == o); }

    private:
        AIt aIt;
        BIt bIt;
    };

    constexpr Zip() = default;
    constexpr explicit Zip(A& a, B& b) : a(&a), b(&b) {}

    constexpr auto begin() const -> iterator { return {adlBegin(*a), adlBegin(*b)}; }
    constexpr auto end() const -> iterator { return {adlEnd(*a), adlEnd(*b)}; }

private:
    A* a{};
    B* b{};
};

template<class A, class B> Zip(A&, B&) -> Zip<A, B>;

} // namespace array19
