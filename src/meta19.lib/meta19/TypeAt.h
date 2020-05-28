#pragma once
#include "Index.h"
#include "nullptr_to.h"

#include <utility> // std::index_sequence

namespace meta19 {

template<size_t, class> struct IndexType {};

template<class IP, class... Ts> struct IndexTypeOverloads;
template<size_t... Is, class... Ts>
struct IndexTypeOverloads<std::index_sequence<Is...>, Ts...> : IndexType<Is, Ts>... {};

template<class... Ts> using IndexTypeMap = IndexTypeOverloads<std::make_index_sequence<sizeof...(Ts)>, Ts...>;

template<size_t I, class T> auto typeAtMap(IndexType<I, T>*) -> T;
template<size_t I, class... Ts> using TypeAt = decltype(typeAtMap<I>(nullptr_to<IndexTypeMap<Ts...>>));
template<size_t I, class ITM> using TypeAtMap = decltype(typeAtMap<I>(nullptr_to<ITM>));

template<class T, size_t I> auto indexTypeOfMap(IndexType<I, T>*) -> Index<I>;
template<class T, class... Ts> using IndexOf = decltype(indexTypeOfMap<T>(nullptr_to<IndexTypeMap<Ts...>>));
template<class T, class ITM> using IndexOfMap = decltype(indexTypeOfMap<T>(nullptr_to<ITM>));

template<class T, size_t I> constexpr auto indexOfMap(IndexType<I, T>*) -> size_t { return I; }
template<class T, class... Ts> inline constexpr auto index_of = indexOfMap<T>(nullptr_to<IndexTypeMap<Ts...>>);
template<class T, class ITM> inline constexpr auto index_of_map = indexOfMap<T>(nullptr_to<ITM>);

} // namespace meta19
