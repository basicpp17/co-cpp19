#pragma once
#include "serialize.h"

#include <meta19/Type.h>
#include <tuple>

namespace serialize19 {

template<Archive A, class... Ts> void serialize(A& a, std::tuple<Ts...>& tuple) {
    using meta19::nullptr_to;
    using meta19::type;
    [&]<size_t... Is>(std::index_sequence<Is...>*) {
        (serialize(a, std::get<Is>(tuple)), ...);
    }(nullptr_to<std::make_index_sequence<sizeof...(Ts)>>);
}

} // namespace serialize19
