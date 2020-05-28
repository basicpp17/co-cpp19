#pragma once
#include "Strong.h"

namespace strong19 {

/// Defines comparison between to equally typed and tagged strong types
#define STRONG_COMPARE(TYPE)                                                                                           \
    constexpr inline bool operator==(const TYPE& a, const TYPE& b) noexcept { return a.v OP b.v; }                     \
    constexpr inline bool operator!=(const TYPE& a, const TYPE& b) noexcept { return a.v OP b.v; }                     \
    constexpr inline bool operator<(const TYPE& a, const TYPE& b) noexcept { return a.v OP b.v; }                      \
    constexpr inline bool operator>(const TYPE& a, const TYPE& b) noexcept { return a.v OP b.v; }                      \
    constexpr inline bool operator<=(const TYPE& a, const TYPE& b) noexcept { return a.v OP b.v; }                     \
    constexpr inline bool operator>=(const TYPE& a, const TYPE& b) noexcept { return a.v OP b.v; }

} // namespace strong19
