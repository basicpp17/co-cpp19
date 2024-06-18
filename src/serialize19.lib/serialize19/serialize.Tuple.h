#pragma once
#include "serialize.h"
#include "tuple19/Tuple.h" // requires tuple19

namespace serialize19 {

template<Archive A, class... Ts> void serialize(A& a, tuple19::Tuple<Ts...>& tuple) {
    using meta19::nullptr_to;
    using meta19::type;
    [&]<size_t... Is>(std::index_sequence<Is...>*) {
        (serialize(a, tuple.template amendAt<Is>()), ...);
    }(nullptr_to<std::make_index_sequence<sizeof...(Ts)>>);
}

} // namespace serialize19
