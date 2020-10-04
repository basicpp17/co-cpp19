#pragma once
#include "ADL.h"
#include "StringView.h"

namespace string19 {

constexpr auto ropeLengthOf(ADL*, StringView sv) -> size_t { return sv.count; }

constexpr void ropeAppend(ADL*, char*& data, StringView sv) {
    for (auto i = 0u; i < sv.count; ++i) *data++ = sv[i];
}

constexpr bool ropeCombine(ADL*, StringView& sv, StringView ov) {
    if (sv.data + sv.count == ov.data) {
        sv.count += ov.count;
        sv.extraZero = ov.extraZero;
        return true;
    }
    return false;
}

} // namespace string19
