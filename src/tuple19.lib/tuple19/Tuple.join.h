#pragma once
#include "Tuple.h"

namespace tuple19 {

/// returns a tuple with all values of a and b
/// note: types don't have to be uniqe
template<class... As, class... Bs>
auto joinTuples(Tuple<As...> const& a, Tuple<Bs...> const& b) -> Tuple<As..., Bs...> {
    return [&]<std::size_t... AIs, size_t... BIs>(
               std::index_sequence<AIs...> const&,
               std::index_sequence<BIs...> const&)
        ->Tuple<As..., Bs...> {
        return {a.template at<AIs>()..., b.template at<BIs>()...};
    }
    (std::make_index_sequence<sizeof...(As)>{}, std::make_index_sequence<sizeof...(Bs)>{});
}

template<class... As, class... Bs> auto joinTuples(Tuple<As...>&& a, Tuple<Bs...>&& b) -> Tuple<As..., Bs...> {
    return [&]<std::size_t... AIs, size_t... BIs>(
               std::index_sequence<AIs...> const&,
               std::index_sequence<BIs...> const&)
        ->Tuple<As..., Bs...> {
        return {std::move(a.template amendAt<AIs>())..., std::move(b.template amendAt<BIs>())...};
    }
    (std::make_index_sequence<sizeof...(As)>{}, std::make_index_sequence<sizeof...(Bs)>{});
}

} // namespace tuple19
