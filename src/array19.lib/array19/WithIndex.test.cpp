#include "WithIndex.h"

using namespace array19;

void constexpr_WithIndex_test() {
    constexpr auto sum = [] {
        int a[] = {1, 2, 3};
        auto r = 0;
        for (auto [v, i] : WithIndex{a}) {
            r += v * i;
        }
        return r;
    }();
    static_assert(sum == (1 * 0) + (2 * 1) + (3 * 2));
}
