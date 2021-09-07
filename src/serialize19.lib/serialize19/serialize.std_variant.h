#pragma once
#include "serialize.h"

#include <stddef.h> // size_t
#include <meta19/nullptr_to.h>
#include <variant>

namespace serialize19 {

using meta19::nullptr_to;

template<Archive A, class... Ts> void serialize(A& a, std::variant<Ts...>& variant) {
    auto index = static_cast<uint32_t>(variant.index());
    serialize(a, index);

    [&, index ]<class... Vs, size_t... Is>(std::variant<Vs...>*, std::index_sequence<Is...>*) {
        ((index == Is ? (
                            [&]<class Value>(Value*) {
                                if constexpr (A::mode == ArchiveMode::Read) {
                                    auto value = Value{};
                                    serialize(a, value);
                                    variant = value;
                                }
                                else {
                                    auto& value = *std::get_if<Value>(&variant);
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
