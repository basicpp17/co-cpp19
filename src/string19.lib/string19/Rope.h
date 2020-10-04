#pragma once
#include "meta19/RemoveReference.h"
#include "tuple19/Tuple.h"

namespace string19 {

using meta19::StoredOf;
using tuple19::Tuple;

/// static combination of nested parts that can be combined to a string at compile time
/// usage:
/// * code generation of templated types
/// see: Rope.store.h for usage example
template<class... Ts> struct Rope;

template<class... Ts> requires(sizeof...(Ts) > 0) struct Rope<Ts...> {
    Tuple<Ts...> parts;
    constexpr Rope() = default;
    constexpr explicit Rope(const Ts&... ts) : parts(ts...) {}
    constexpr explicit Rope(Ts&&... ts) : parts(std::move(ts)...) {}
    constexpr auto operator==(const Rope&) const -> bool = default;
};

template<> struct Rope<> {};

template<class... Ts> Rope(Ts&&...) -> Rope<StoredOf<Ts>...>;

} // namespace string19
