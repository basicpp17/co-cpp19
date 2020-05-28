#pragma once
#include "Tuple.h"
#include "meta19/Index.h"

namespace tuple19 {

using meta19::_index;

template<size_t I, class... Ts> auto constexpr get(Tuple<Ts...>& tuple) -> decltype(auto) {
    return tuple.amendAt(_index<I>);
}
template<size_t I, class... Ts> auto constexpr get(Tuple<Ts...>&& tuple) -> decltype(auto) {
    return std::move(tuple.amendAt(_index<I>));
}
template<size_t I, class... Ts> auto constexpr get(const Tuple<Ts...>& tuple) -> decltype(auto) {
    return tuple.at(_index<I>);
}
template<class T, class... Ts> auto constexpr get(Tuple<Ts...>& tuple, Type<T>* = {}) -> decltype(auto) {
    return tuple.amendOf(type<T>);
}
template<class T, class... Ts> auto constexpr get(Tuple<Ts...>&& tuple, Type<T>* = {}) -> decltype(auto) {
    return std::move(tuple.amendOf(type<T>));
}
template<class T, class... Ts> auto constexpr get(const Tuple<Ts...>& tuple, Type<T>* = {}) -> decltype(auto) {
    return tuple.of(type<T>);
}

} // namespace tuple19

/// for c++17 structured bindings support
namespace std {

template<class... Ts> class tuple_size<tuple19::Tuple<Ts...>> : public std::integral_constant<size_t, sizeof...(Ts)> {};

template<size_t I, class... Ts> class tuple_element<I, tuple19::Tuple<Ts...>> {
public:
    using type = typename tuple19::Tuple<Ts...>::template At<I>;
};

} // namespace std
