#pragma once
#include "Span.h"

namespace array19 {

template<class T> constexpr auto spanMaximum(Span<T> span) {
    auto result = T{};
    for (auto v : span) {
        if (v > result) result = v;
    }
    return result;
}

} // namespace array19
