#pragma once
#include "Tuple.h"

namespace tuple19 {

template<class... Ts> bool operator==(const Tuple<Ts...>& a, const Tuple<Ts...>& b) {
    return ((a.of(type<Ts>) == b.of(type<Ts>)) && ...);
}

template<class... Ts> bool operator!=(const Tuple<Ts...>& a, const Tuple<Ts...>& b) { return !(a == b); }

} // namespace tuple19
