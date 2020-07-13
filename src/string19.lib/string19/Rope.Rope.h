#pragma once
#include "ADL.h"
#include "Rope.h"

namespace string19 {

template<class... Ts> constexpr auto ropeAppend(ADL*, char*& data, const Rope<Ts...>& rope) {
    rope.parts.visitAll([&](const auto& p) { ropeAppend(adl, data, p); });
}

template<class... Ts> constexpr auto ropeCount(ADL*, const Rope<Ts...>& rope) -> size_t {
    auto result = size_t{};
    rope.parts.visitAll([&](const auto& p) { result += ropeCount(adl, p); });
    return result;
}

} // namespace string19
