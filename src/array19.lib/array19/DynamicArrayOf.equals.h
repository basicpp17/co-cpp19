#include "DynamicArrayOf.h"

namespace array19 {

template<class T> bool operator==(const DynamicArrayOf<T>& a, const DynamicArrayOf<T>& b) noexcept {
    if (a.count() != b.count()) return false;
    for (auto [av, bv] : Zip(a, b)) {
        if (!(av == bv)) return false;
    }
    return true;
}

template<class T> constexpr bool operator!=(const DynamicArrayOf<T>& a, const DynamicArrayOf<T>& b) {
    return !(a == b);
}

} // namespace array19