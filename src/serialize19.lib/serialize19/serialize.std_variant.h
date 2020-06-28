#pragma once
#include "serialize.h"

#include <meta19/nullptr_to.h>
#include <variant>

namespace serialize19 {

using meta19::nullptr_to;

template<Archive A, class... Ts> void serialize(A& a, std::variant<Ts...>& variant) {
    auto index = variant.index();
    serialize(a, index);

    constexpr static auto handleT = []<class T>(auto& a, auto& variant, T*) {
        if (a.mode == ArchiveMode::Read) {
            auto value = T{};
            serialize(a, value);
            variant = value;
        }
        else {
            auto& value = *std::get_if<T>(&variant);
            serialize(a, value);
        }
    };

    [index]<class... Vs, size_t... Is>(auto& a, std::variant<Vs...>& variant, std::index_sequence<Is...>*) {
        ((index == Is ? (handleT(a, variant, nullptr_to<Vs>), 0) : 0), ...); //
    }
    (a, variant, nullptr_to<std::make_index_sequence<sizeof...(Ts)>>);
}

} // namespace serialize19
