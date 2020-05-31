#pragma once
#include "ADL.h"
#include "meta19/TypePack.h"
#include "string19/StringView.literal.h"

#include <compare>

namespace strong19 {

/// true if T was defined as a strong type
template<class T> constexpr inline auto is_strong = isStrong(nullptr_to<ADL>, nullptr_to<T>);

/// view to the name of the strong type T (is_strong<T> == true)
template<class T> constexpr inline auto strong_name = strongName(nullptr_to<T>);

/// private base class for a strong type
/// note: required or constexpr would be an error for non-constexpr types (like std::string)
template<class V> struct Weak {
    static_assert(!is_strong<V>);
    V v;
    constexpr Weak() = default;
    constexpr explicit Weak(V v) : v(v) {}
    bool operator==(const Weak&) const = default;
};

#ifndef DEFINE_STRONG_EXTRAS
#    define DEFINE_STRONG_EXTRAS(NAME)
#endif

/// Defines a strong value type with name @param NAME that stores value of type @param VALUE and optional tags
#define DEFINE_STRONG(NAME, VALUE, ...)                                                                                \
    struct NAME : private strong19::Weak<VALUE> {                                                                      \
        using Weak::Weak;                                                                                              \
        using Weak::v;                                                                                                 \
        bool operator==(const NAME&) const = default;                                                                  \
    };                                                                                                                 \
    constexpr inline auto isStrong(strong19::ADL*, NAME*)->bool { return true; }                                       \
    auto strongValueType(NAME*)->VALUE;                                                                                \
    auto strongTags(NAME*)->meta19::TypePack<__VA_ARGS__>;                                                             \
    constexpr inline auto strongName(NAME*)->string19::StringView { return string19::viewLiteral(#NAME); }             \
    DEFINE_STRONG_EXTRAS(NAME)                                                                                         \
    struct NAME

/// Fallback for any type that does not have isStrong overload
/// note: This overload is found through argument dependent lookup of strong19::ADL
constexpr inline auto isStrong(ADL*, void*) -> bool { return false; }

/// stored type of the strong type T (is_strong<T> == true)
template<class T> using StrongValueType = decltype(strongValueType(nullptr_to<T>));

/// typepack with all tags of the strong type T (is_strong<T> == true)
template<class T> using StrongTags = decltype(strongTags(nullptr_to<T>));

} // namespace strong19
