#pragma once
#include "ADL.h"
#include "Rope.h"
#include "StringStore.h"
#include "StringView.h"

namespace string19 {

constexpr auto ropeCount(ADL*, char) -> size_t { return 1; }
constexpr auto ropeCount(ADL*, StringView sv) -> size_t { return sv.count; }
template<size_t N> constexpr auto ropeCount(ADL*, const StringStore<N>&) -> size_t { return N; }

template<class... Ts> constexpr auto ropeCount(ADL*, const Rope<Ts...>& rope) -> size_t {
    auto result = size_t{};
    rope.parts.visitAll([&](const auto& p) { result += ropeCount(adl, p.v); });
    return result;
}

template<class T> constexpr auto ropeCount(const T& v) -> size_t { return ropeCount(adl, v); }

} // namespace string19
