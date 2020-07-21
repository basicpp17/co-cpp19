#pragma once
#include "meta19/nullptr_to.h"
#include "serialize.h"
#include "variant19/Variant.h" // requires variant19

namespace serialize19 {

template<Archive A, class... Ts> void serialize(A& a, variant19::Variant<Ts...>& variant) {
    using meta19::nullptr_to;
    using meta19::type;
    using T = variant19::Variant<Ts...>;
    using Which = typename T::Which;
    auto whichValue = static_cast<typename Which::Value>(variant.which());
    serialize(a, whichValue);

    constexpr static auto handleT = []<class T>(auto& a, auto& variant, T*) {
        if constexpr (A::mode == ArchiveMode::Read) {
            auto value = T{};
            serialize(a, value);
            variant = value;
        }
        else {
            auto& value = variant.as(type<T>);
            serialize(a, value);
        }
    };

    [whichValue]<class... Vs, size_t... Is>(auto& a, variant19::Variant<Vs...>& variant, std::index_sequence<Is...>*) {
        ((whichValue == Is ? (handleT(a, variant, nullptr_to<Vs>), 0) : 0), ...); //
    }
    (a, variant, nullptr_to<std::make_index_sequence<sizeof...(Ts)>>);
}

} // namespace serialize19
