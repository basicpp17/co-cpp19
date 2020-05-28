#pragma once
#include "SliceOf.h"
#include "Zip.h"

namespace array19 {

template<class T> constexpr bool operator==(const SliceOf<T>& a, const SliceOf<T>& b) {
    if (a.count() != b.count()) return false;
    for (auto [av, bv] : Zip(a, b)) {
        if (!(av == bv)) return false;
    }
    return true;
}

template<class T> constexpr bool operator!=(const SliceOf<T>& a, const SliceOf<T>& b) { return !(a == b); }

} // namespace array19
