#pragma once
#include "Rope.count.h"

namespace string19 {

constexpr auto ropeAppend(ADL*, char*& data, char c) { *data++ = c; }
constexpr auto ropeAppend(ADL*, char*& data, StringView sv) {
    for (auto i = 0u; i < sv.count; ++i) *data++ = sv[i];
}
template<size_t N> constexpr auto ropeAppend(ADL*, char*& data, const StringStore<N>& s) {
    for (auto i = 0u; i < N; ++i) *data++ = s[i];
}

template<class... Ts> constexpr auto ropeAppend(ADL*, char*& data, const Rope<Ts...>& rope) {
    rope.parts.visitAll([&](const auto& p) { ropeAppend(adl, data, p); });
}

/// @returns a StringStore that concats all parts of the Rope
/// usage:
///    constexpr auto rope = Rope{};
///    constexpr auto N = ropeCount(rope);
///    constexpr auto store = ropeStore<N>(rope);
template<size_t N, class... Ts> constexpr auto ropeStore(const Rope<Ts...>& r) -> const StringStore<N> {
    auto result = StringStore<N>{};
    auto* data = result.data;
    ropeAppend(adl, data, r);
    return result;
}

} // namespace string19
