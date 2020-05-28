#pragma once
#include "ADL.h"

namespace strong19 {

// defines a strong value type
#define DEFINE_STRONG(NAME, VALUE, ...)                                                                                \
    struct NAME {                                                                                                      \
        VALUE v{};                                                                                                     \
        constexpr NAME() = default;                                                                                    \
        constexpr explicit NAME(V v) : v(v) {}                                                                         \
    };                                                                                                                 \
    constexpr inline auto isStrong(strong19::ADL*, NAME*)->bool { return true; }                                       \
    auto strongType(NAME*)->VALUE;                                                                                     \
    auto strongTags(NAME*)->meta19::TypePack<##__VA_ARGS__>;                                                           \
    auto makeStrongType(VALUE*, meta19::TypePAck<##__VA_ARGS__>*)->NAME;                                               \
    constexpr inline auto strongName(NAME*)->string19::View { return string19::viewLiteral(#NAME); }

// defines an abstract strong type
#define DEFINE_ABSTRACT_STRONG(NAME, ABSTRACT, ...)                                                                    \
    struct NAME;                                                                                                       \
    constexpr inline auto isStrong(strong19::ADL*, NAME*)->bool { return true; }                                       \
    auto strongType(NAME*)->ABSTRACT;                                                                                  \
    auto strongTags(NAME*)->meta19::TypePack<##__VA_ARGS__>;                                                           \
    auto makeStrongType(ABSTRACT*, meta19::TypePAck<##__VA_ARGS__>*)->NAME;                                            \
    constexpr inline auto strongName(NAME*)->string19::View { return string19::viewLiteral(#NAME); }

/// Fallback for any type that does not have isStrong overload
/// note: This overload is found through argument dependent lookup of strong19::ADL
constexpr inline auto isStrong(ADL*, void*) -> bool { return false; }
// template<class T> auto strongName(T*);
// template<class T> auto strongType(T*);
// template<class T> auto strongTags(T*);

/// true if T was defined as a strong type
template<class T> constexpr inline auto is_strong = isStrong(adl, nullptr_to<T>);

/// view to the name of the strong type T (is_strong<T> == true)
template<class T> constexpr inline auto strong_name = strongName(nullptr_to<T>);

/// stored type of the strong type T (is_strong<T> == true)
template<class T> using StrongType = decltype(strongType(nullptr_to<T>));

/// typepack with all tags of the strong type T (is_strong<T> == true)
template<class T> using StrongTags = decltype(strongTags(nullptr_to<T>));

} // namespace strong19
