#include "OrderedMapOf.h"

#include "OrderedMapOf.equals.h"
#include "OrderedMapOf.ostream.h"
#include "array19/Array.h"

#include <array19/Array.h>
#include <array19/SliceOf.equals.h>
#include <array19/SliceOf.ostream.h>
#include <gtest/gtest.h>

using namespace lookup19;

TEST(OrderedMapOf, basics) {
    auto v = OrderedMapOf<int, int>{};

    EXPECT_TRUE(v.isEmpty());
    EXPECT_FALSE(v.hasKey(17));
    EXPECT_EQ(v.totalCapacity(), 0u);
    EXPECT_EQ(v.unusedCapacity(), 0u);
    ASSERT_EQ(v.count(), 0u);

    v.ensureUnusedCapacity(1);

    EXPECT_GE(v.totalCapacity(), 1u);
    EXPECT_GE(v.unusedCapacity(), 1u);
    ASSERT_EQ(v.count(), 0u);

    v.insert(12, 1);
    v.insert(23, 2);
    v.insert(17, 3);

    ASSERT_EQ(v.count(), 3u);
    EXPECT_TRUE(v.hasKey(17));
    EXPECT_FALSE(v.hasKey(42));
    EXPECT_GE(v.totalCapacity(), 3u);
    EXPECT_EQ(v.keys(), static_cast<array19::SliceOf<int const>>(array19::Array{12, 17, 23}));
    EXPECT_EQ(
        static_cast<array19::SliceOf<int const>>(v.values()),
        static_cast<array19::SliceOf<int const>>(array19::Array{1, 3, 2}));

    v.insert(42, 4);
    v.insert(12, 5);

    ASSERT_EQ(v.count(), 4u);
    EXPECT_TRUE(v.hasKey(42));
    EXPECT_GE(v.totalCapacity(), 4u);
    EXPECT_EQ(
        static_cast<array19::SliceOf<int const>>(v.orderedKeys()),
        static_cast<array19::SliceOf<int const>>(array19::Array{12, 17, 23, 42}));
    EXPECT_EQ(
        static_cast<array19::SliceOf<int const>>(v.values()),
        static_cast<array19::SliceOf<int const>>(array19::Array{1, 3, 2, 4}));

    v.remove(OrderedSliceOf{v.orderedKeys()}.lowerBound(17));

    ASSERT_EQ(v.count(), 3u);
    EXPECT_FALSE(v.hasKey(17));
    EXPECT_EQ(
        static_cast<array19::SliceOf<int const>>(v.orderedKeys()),
        static_cast<array19::SliceOf<int const>>(array19::Array{12, 23, 42}));
    EXPECT_EQ(
        static_cast<array19::SliceOf<int const>>(v.values()),
        static_cast<array19::SliceOf<int const>>(array19::Array{1, 2, 4}));

    auto v2 = v;
    EXPECT_EQ(v, v2);
    v.insert(13, 5);
    EXPECT_NE(v, v2);
    v2.emplace(13, 7);
    EXPECT_NE(v, v2);

    *v.amendBy(13) = 7;
    EXPECT_EQ(v, v2);
}

using array19::Array;

TEST(OrderedMapOf, intoMerge) {
    using IM = OrderedMapOf<int, int>;

    // clang-format off
    auto aKeys = Array{   1,  3,  7};
    auto aValues = Array{10, 11, 12};
    // clang-format on
    auto a = IM{OrderedSliceOf<int const, DefaultLess>{aKeys}, aValues.begin()};

    // clang-format off
    auto bKeys = Array{   2,  3,  4,  5,  8};
    auto bValues = Array{20, 21, 22, 23, 24};
    // clang-format on
    // auto b = IM(OrderedSliceOf<int const, DefaultLess>{bKeys}, bValues.begin());

    a.mergeRange(OrderedSliceOf<int const, DefaultLess>{bKeys}, bValues.begin());

    // clang-format off
    auto mKeys = Array{   1,  2,  3,  4,  5,  7,  8};
    auto mValues = Array{10, 20, 21, 22, 23, 12, 24};
    // clang-format on
    auto m = IM{OrderedSliceOf<int const, DefaultLess>{mKeys}, mValues.begin()};
    EXPECT_EQ(a, m);
}

TEST(OrderedMapOf, intoMerge2) {
    using IM = OrderedMapOf<int, int>;

    // clang-format off
    auto aKeys = Array{   2,  3,  4,  5,  8};
    auto aValues = Array{20, 21, 22, 23, 24};
    // clang-format on
    auto a = IM{OrderedSliceOf<int const, DefaultLess>{aKeys}, aValues.begin()};

    // clang-format off
    auto bKeys = Array{   1,  3,  7};
    auto bValues = Array{10, 11, 12};
    // clang-format on
    // auto b = IM(OrderedSliceOf<int const, DefaultLess>{bKeys}, bValues.begin());

    a.mergeRange(OrderedSliceOf<int const, DefaultLess>{bKeys}, bValues.begin());

    // clang-format off
    auto mKeys = Array{   1,  2,  3,  4,  5,  7,  8};
    auto mValues = Array{10, 20, 11, 22, 23, 12, 24};
    // clang-format on
    auto m = IM{OrderedSliceOf<int const, DefaultLess>{mKeys}, mValues.begin()};
    EXPECT_EQ(a, m);
}

TEST(OrderedMapOf, inplaceMerge) {
    using IM = OrderedMapOf<int, int>;

    auto a = IM{};
    // clang-format off
    auto aKeys = Array{   1,  3,  7};
    auto aValues = Array{10, 11, 12};
    // clang-format on
    a.mergeRange(OrderedSliceOf<int const, DefaultLess>{aKeys}, aValues.begin());
    a.ensureUnusedCapacity(5);

    // clang-format off
    auto bKeys = Array{   2,  3,  4,  5,  8};
    auto bValues = Array{20, 21, 22, 23, 24};
    // clang-format on
    a.mergeRange(OrderedSliceOf<int const, DefaultLess>{bKeys}, bValues.begin());

    // clang-format off
    auto mKeys = Array{   1,  2,  3,  4,  5,  7,  8};
    auto mValues = Array{10, 20, 21, 22, 23, 12, 24};
    // clang-format on
    auto m = IM{OrderedSliceOf<int const, DefaultLess>{mKeys}, mValues.begin()};
    EXPECT_EQ(a, m);
}

TEST(OrderedMapOf, inplaceMerge2) {
    using IM = OrderedMapOf<int, int>;

    auto a = IM{};
    // clang-format off
    auto aKeys = Array{   2,  3,  4,  5,  8};
    auto aValues = Array{20, 21, 22, 23, 24};
    // clang-format on
    a.mergeRange(OrderedSliceOf<int const, DefaultLess>{aKeys}, aValues.begin());
    a.ensureUnusedCapacity(5);

    // clang-format off
    auto bKeys = Array{   1,  3,  7};
    auto bValues = Array{10, 11, 12};
    // clang-format on
    a.mergeRange(OrderedSliceOf<int const, DefaultLess>{bKeys}, bValues.begin());

    // clang-format off
    auto mKeys = Array{   1,  2,  3,  4,  5,  7,  8};
    auto mValues = Array{10, 20, 11, 22, 23, 12, 24};
    // clang-format on
    auto m = IM{OrderedSliceOf<int const, DefaultLess>{mKeys}, mValues.begin()};
    EXPECT_EQ(a, m);
}

TEST(OrderedMapOf, inplaceMergeAllFront) {
    using IM = OrderedMapOf<int, int>;

    auto a = IM{};
    // clang-format off
    auto aKeys = Array{   8, 11, 13, 17};
    auto aValues = Array{ 9, 10, 11, 12};
    // clang-format on
    a.mergeRange(OrderedSliceOf<int const, DefaultLess>{aKeys}, aValues.begin());
    a.ensureUnusedCapacity(5);

    // clang-format off
    auto bKeys = Array{   2,  3,  4,  5,  8};
    auto bValues = Array{20, 21, 22, 23, 24};
    // clang-format on
    a.mergeRange(OrderedSliceOf<int const, DefaultLess>{bKeys}, bValues.begin());

    // clang-format off
    auto mKeys = Array{   2,  3,  4,  5,  8, 11, 13, 17};
    auto mValues = Array{20, 21, 22, 23, 24, 10, 11, 12};
    // clang-format on
    auto m = IM{OrderedSliceOf<int const, DefaultLess>{mKeys}, mValues.begin()};
    EXPECT_EQ(a, m);
}
