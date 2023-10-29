#pragma once
#include "AllocatedArray.h"
#include "Zip.h"

namespace array19 {

template<class T> bool operator==(AllocatedArray<T> const& a, AllocatedArray<T> const& b) noexcept {
    if (a.count() != b.count()) return false;
    for (auto [av, bv] : Zip(a, b)) {
        if (!(av == bv)) return false;
    }
    return true;
}

} // namespace array19
