#pragma once
#include "DynamicArray.h"
#include "Zip.h"

namespace array19 {

template<class T> bool operator==(DynamicArray<T> const& a, DynamicArray<T> const& b) noexcept {
    if (a.count() != b.count()) return false;
    for (auto [av, bv] : Zip(a, b)) {
        if (!(av == bv)) return false;
    }
    return true;
}

template<class T> bool operator!=(DynamicArray<T> const& a, DynamicArray<T> const& b) noexcept { return !(a == b); }

} // namespace array19
