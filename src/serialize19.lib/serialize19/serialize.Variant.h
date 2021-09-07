#pragma once
#include "meta19/nullptr_to.h"
#include "serialize.h"
#include "variant19/Variant.h" // requires variant19

#include <stddef.h> // size_t

namespace serialize19 {

template<Archive A, class... Ts> void serialize(A& a, variant19::Variant<Ts...>& variant) {
    using meta19::nullptr_to;
    using meta19::type;
    using T = variant19::Variant<Ts...>;
    using Which = typename T::Which;
    auto whichValue = static_cast<typename Which::Value>(variant.which());
    serialize(a, whichValue);

    [&, whichValue ]<class... Vs, size_t... Is>(variant19::Variant<Vs...>*, std::index_sequence<Is...>*) {
        ((whichValue == Is ? (
                                 [&]<class Value>(Value*) {
                                     if constexpr (A::mode == ArchiveMode::Read) {
                                         auto value = Value{};
                                         serialize(a, value);
                                         variant = value;
                                     }
                                     else {
                                         auto& value = variant.as(type<Value>);
                                         serialize(a, value);
                                     }
                                 }(nullptr_to<Vs>),
                                 0)
                           : 0),
         ...);
    }
    (&variant, nullptr_to<std::make_index_sequence<sizeof...(Ts)>>);
}

} // namespace serialize19
