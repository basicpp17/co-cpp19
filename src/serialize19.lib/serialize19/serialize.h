#pragma once
#include "meta19/isSame.h"

#include <type_traits> // std::is_arithmetic_v, std::is_enum_v

namespace serialize19 {

struct SerializeFallback {
    template<class T> SerializeFallback(const T&) {
        static_assert(sizeof(T) != sizeof(T), "Missing serialize overload!");
    }
};
template<class Archive> void serialize(Archive&, SerializeFallback) {}

template<class T> constexpr auto is_serialize_primitive = std::is_arithmetic_v<T> || std::is_enum_v<T>;

template<class Archive, class T> requires(is_serialize_primitive<T>) auto serialize(Archive& a, T& v) {
    a.withPrimitive(v);
}

} // namespace serialize19
