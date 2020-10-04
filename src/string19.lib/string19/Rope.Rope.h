#pragma once
#include "ADL.h"
#include "Rope.h"

namespace string19 {

template<class... Ts> constexpr auto ropeLengthOf(ADL*, const Rope<Ts...>& rope) -> size_t {
    auto result = size_t{};
    rope.parts.visitAll([&](const auto& p) { result += ropeLengthOf(adl, p); });
    return result;
}
template<> constexpr auto ropeLengthOf(ADL*, const Rope<>&) -> size_t { return {}; }

template<class... Ts> constexpr void ropeAppend(ADL*, char*& data, const Rope<Ts...>& rope) {
    rope.parts.visitAll([&](const auto& p) { ropeAppend(adl, data, p); });
}
template<> constexpr void ropeAppend(ADL*, char*&, const Rope<>&) {}

} // namespace string19
