#include "Array.slice.h"
#include "SliceOf.carray.h"
#include "SliceOf.h"

using namespace array19;

void constexpr_SliceOf_test() {
    constexpr static int a[3] = {1, 2, 3};
    constexpr auto s = sliceOfCArray(a);

    static_assert(s.count() == 3);
    static_assert(s.at(1) == 2);

    constexpr auto sum = [&] {
        auto r = 0;
        for (auto v : s) r += v;
        return r;
    }();
    static_assert(sum == 6);
}

void mutable_SliceOf_test() {
    constexpr auto sa = [] {
        int a[] = {1, 2, 3};
        auto s = amendSliceOfCArray(a);
        s.at(1) = 5;
        return arrayFrom(s, static_cast<std::make_index_sequence<3>*>(nullptr));
    }();

    static_assert(sa.at(1) == 5);
}
