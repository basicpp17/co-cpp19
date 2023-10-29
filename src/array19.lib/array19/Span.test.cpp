#include "Span.h"

#include "Span.carray.h"
#include "Span.store.h"

#include <gtest/gtest.h>

using namespace array19;

void constexpr_Span_test() {
    constexpr static int a[3] = {1, 2, 3};
    constexpr auto s = spanOfCArray(a);

    static_assert(s.count() == 3);
    static_assert(s[1] == 2);

    constexpr auto sum = [&] {
        auto r = 0;
        for (auto v : s) r += v;
        return r;
    }();
    static_assert(sum == 6);
}

TEST(Span, Access) {
    int a[3] = {1, 2, 3};
    auto s = spanOfCArray(a);

    EXPECT_EQ(s.count(), 3u);
    EXPECT_EQ(s[1], 2);

    auto sum = 0;
    for (auto v : s) sum += v;
    EXPECT_EQ(sum, 6);
}

void mutable_Span_test() {
    constexpr auto sa = [] {
        int a[] = {1, 2, 3};
        auto s = amendSpanOfCArray(a);
        s[1] = 5;
        return storeSpan<3>(s);
    }();

    static_assert(sa[1] == 5);
}

TEST(Span, Mutate) {
    int a[] = {1, 2, 3, 4};
    auto s = amendSpanOfCArray(a);
    s[1] = 5;
    EXPECT_EQ(a[1], 5);

    auto sa = storeSpan<4>(s);
    EXPECT_EQ(sa[1], 5);
    EXPECT_EQ(sa[3], 4);
}
