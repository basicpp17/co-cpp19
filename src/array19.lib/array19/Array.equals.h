#pragma once
#include "Array.h"
#include "Zip.h"

namespace array19 {

template<class T, size_t AN, size_t BN>
constexpr bool operator==(const Array<T, AN>& a, const Array<T, BN>& b) noexcept {
    if constexpr (AN != BN)
        return false;
    else {
        for (auto [av, bv] : Zip(a, b)) {
            if (!(av == bv)) return false;
        }
        return true;
    }
}

template<class T, size_t AN, size_t BN>
constexpr bool operator!=(const Array<T, AN>& a, const Array<T, BN>& b) noexcept {
    return !(a == b);
}

} // namespace array19
