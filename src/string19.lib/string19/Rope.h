#pragma once
#include "meta19/RemoveReference.h"
#include "tuple19/Tuple.h"

namespace string19 {

using meta19::StoredOf;
using tuple19::Tuple;

template<class... Ts> struct Rope {
    Tuple<Ts...> parts;
    constexpr Rope() = default;
    constexpr Rope(const Ts&... ts) : parts(ts...) {}
    constexpr Rope(Ts&&... ts) : parts(std::move(ts)...) {}
};

template<class... Ts> Rope(Ts&&...)->Rope<StoredOf<Ts>...>;

} // namespace string19
