#include "OrderedSetOf.h"

#include "array19/Array.h"
#include "array19/SliceOf.equals.h"
#include "array19/SliceOf.ostream.h"

#include <gtest/gtest.h>

using namespace lookup19;
using array19::Array;
using array19::SliceOf;

TEST(OrderedSetOf, intExample) {
    using Set = OrderedSetOf<int>;
    using OrderedSlice = Set::Slice;

    auto v = Set{};

    EXPECT_TRUE(v.isEmpty());
    EXPECT_EQ(v.totalCapacity(), 0u);
    EXPECT_EQ(v.unusedCapacity(), 0u);
    ASSERT_EQ(v.count(), 0u);

    v.ensureUnusedCapacity(1);

    EXPECT_GE(v.totalCapacity(), 1u);
    EXPECT_GE(v.unusedCapacity(), 1u);
    ASSERT_EQ(v.count(), 0u);
    ASSERT_FALSE(OrderedSlice{v}.has(12));

    v.insert(12);
    v.insert(23);

    ASSERT_EQ(v.count(), 2u);
    EXPECT_GE(v.totalCapacity(), 2u);
    ASSERT_FALSE(OrderedSlice{v}.has(11));
    ASSERT_TRUE(OrderedSlice{v}.has(12));
    ASSERT_FALSE(OrderedSlice{v}.has(22));
    ASSERT_TRUE(OrderedSlice{v}.has(23));
    ASSERT_FALSE(OrderedSlice{v}.has(24));

    v.insert(42);
    v.insert(12);

    ASSERT_EQ(v.count(), 3u);
    EXPECT_GE(v.totalCapacity(), 3u);

    v.remove(OrderedSliceOf{v}.lowerBound(12));

    ASSERT_EQ(v.count(), 2u);
}

TEST(OrderedSetOf, unorderedInsert) {
    using Set = OrderedSetOf<int>;
    using OrderedSlice = Set::Slice;
    using Slice = array19::SliceOf<const int>;
    auto asSlice = [](auto& x) -> Slice { return OrderedSlice{x}; };

    auto v = Set{};

    v.insert(2);
    ASSERT_EQ(asSlice(v), (Slice{Array{2}}));

    v.insert(1);
    ASSERT_EQ(asSlice(v), (Slice{Array{1, 2}}));

    v.insert(3);
    ASSERT_EQ(asSlice(v), (Slice{Array{1, 2, 3}}));

    v.insert(0);
    ASSERT_EQ(asSlice(v), (Slice{Array{0, 1, 2, 3}}));

    v.insert(5);
    ASSERT_EQ(asSlice(v), (Slice{Array{0, 1, 2, 3, 5}}));

    v.insert(4);
    ASSERT_EQ(asSlice(v), (Slice{Array{0, 1, 2, 3, 4, 5}}));

    v.remove(OrderedSliceOf{v}.lowerBound(2));
    ASSERT_EQ(asSlice(v), (Slice{Array{0, 1, 3, 4, 5}}));

    v.remove(OrderedSliceOf{v}.lowerBound(3));
    ASSERT_EQ(asSlice(v), (Slice{Array{0, 1, 4, 5}}));

    v.remove(OrderedSliceOf{v}.lowerBound(0));
    ASSERT_EQ(asSlice(v), (Slice{Array{1, 4, 5}}));
}

TEST(OrderedSetOf, merge) {
    using Set = OrderedSetOf<int>;
    using OrderedSlice = Set::Slice;
    using Slice = array19::SliceOf<const int>;
    auto asSlice = [](auto& x) -> Slice { return OrderedSlice{x}; };

    auto v1 = Set{OrderedSlice{Array{10, 20, 30}}};
    ASSERT_EQ(asSlice(v1), (Slice{Array{10, 20, 30}}));

    auto v2 = Set{OrderedSlice{Array{5, 10, 11, 12, 30, 35}}};
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 10, 11, 12, 30, 35}}));

    auto v3 = v1;

    v1.merge(v2);
    ASSERT_EQ(asSlice(v1), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));

    v2.merge(v3);
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));
}

TEST(OrderedSetOf, mergeUnique) {
    using Set = OrderedSetOf<int>;
    using OrderedSlice = Set::Slice;
    using Slice = array19::SliceOf<const int>;
    auto asSlice = [](auto& x) -> Slice { return OrderedSlice{x}; };

    auto v1 = Set{OrderedSlice{Array{10, 20, 30}}};
    ASSERT_EQ(asSlice(v1), (Slice{Array{10, 20, 30}}));

    auto v2 = Set{OrderedSlice{Array{5, 11, 12, 35}}};
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 11, 12, 35}}));

    auto v3 = v1;

    v1.merge(v2);
    ASSERT_EQ(asSlice(v1), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));

    v2.merge(v3);
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));
}

TEST(OrderedSetOf, mergeEndDuplicate) {
    using Set = OrderedSetOf<int>;
    using OrderedSlice = Set::Slice;
    using Slice = array19::SliceOf<const int>;
    auto asSlice = [](auto& x) -> Slice { return OrderedSlice{x}; };

    auto v1 = Set{OrderedSlice{Array{10, 20, 30, 35}}};
    ASSERT_EQ(asSlice(v1), (Slice{Array{10, 20, 30, 35}}));

    auto v2 = Set{OrderedSlice{Array{5, 10, 11, 12, 35}}};
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 10, 11, 12, 35}}));

    auto v3 = v1;

    v1.merge(v2);
    ASSERT_EQ(asSlice(v1), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));

    v2.merge(v3);
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));
}

TEST(OrderedSetOf, mergeWithCapacity) {
    using Set = OrderedSetOf<int>;
    using OrderedSlice = Set::Slice;
    using Slice = array19::SliceOf<const int>;
    auto asSlice = [](auto& x) -> Slice { return static_cast<Slice>(static_cast<OrderedSlice>(x)); };

    auto v1 = Set{OrderedSlice{Array{10, 20, 30}}};
    v1.ensureCapacity(10);
    ASSERT_EQ(asSlice(v1), (Slice{Array{10, 20, 30}}));

    auto v2 = Set{OrderedSlice{Array{5, 10, 11, 12, 35}}};
    v2.ensureCapacity(10);
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 10, 11, 12, 35}}));

    auto v3 = v1;

    v1.merge(v2);
    ASSERT_EQ(asSlice(v1), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));

    v2.merge(v3);
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));
}

TEST(OrderedSetOf, mergeUniqueWithCapacity) {
    using Set = OrderedSetOf<int>;
    using OrderedSlice = Set::Slice;
    using Slice = array19::SliceOf<const int>;
    auto asSlice = [](auto& x) -> Slice { return OrderedSlice{x}; };

    auto v1 = Set{OrderedSlice{Array{10, 20, 30}}};
    v1.ensureCapacity(10);
    ASSERT_EQ(asSlice(v1), (Slice{Array{10, 20, 30}}));

    auto v2 = Set{OrderedSlice{Array{5, 11, 12, 35}}};
    v2.ensureCapacity(10);
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 11, 12, 35}}));

    auto v3 = v1;

    v1.merge(v2);
    ASSERT_EQ(asSlice(v1), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));

    v2.merge(v3);
    ASSERT_EQ(asSlice(v2), (Slice{Array{5, 10, 11, 12, 20, 30, 35}}));
}
