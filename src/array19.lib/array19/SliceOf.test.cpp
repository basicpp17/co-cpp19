#include "SliceOf.carray.h"
#include "SliceOf.h"
#include "SliceOf.store.h"

using namespace array19;

void constexpr_SliceOf_test() {
    constexpr static int a[3] = {1, 2, 3};
    constexpr auto s = sliceOfCArray(a);

    static_assert(s.count() == 3);
    static_assert(s[1] == 2);

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
        s[1] = 5;
        return storeSlice<3>(s);
    }();

    static_assert(sa[1] == 5);
}
