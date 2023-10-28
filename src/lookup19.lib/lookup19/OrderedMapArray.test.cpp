#include "OrderedMapArray.h"

#include "OrderedMapArray.equals.h"
#include "OrderedMapArray.ostream.h"
#include "array19/Array.h"
#include "array19/Span.carray.h"
#include "array19/Span.equals.h"
#include "array19/Span.ostream.h"

#include <gtest/gtest.h>

using namespace lookup19;

TEST(OrderedMapArray, basics) {
    auto v = OrderedMapArray<int, int>{};

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
    EXPECT_EQ(v.keys(), static_cast<array19::Span<int const>>(array19::Array{12, 17, 23}));
    EXPECT_EQ(
        static_cast<array19::Span<int const>>(v.values()),
        static_cast<array19::Span<int const>>(array19::Array{1, 3, 2}));

    v.insert(42, 4);
    v.insert(12, 5);

    ASSERT_EQ(v.count(), 4u);
    EXPECT_TRUE(v.hasKey(42));
    EXPECT_GE(v.totalCapacity(), 4u);
    EXPECT_EQ(
        static_cast<array19::Span<int const>>(v.orderedKeys()),
        static_cast<array19::Span<int const>>(array19::Array{12, 17, 23, 42}));
    EXPECT_EQ(
        static_cast<array19::Span<int const>>(v.values()),
        static_cast<array19::Span<int const>>(array19::Array{1, 3, 2, 4}));

    v.remove(OrderedSpan{v.orderedKeys()}.lowerBound(17));

    ASSERT_EQ(v.count(), 3u);
    EXPECT_FALSE(v.hasKey(17));
    EXPECT_EQ(
        static_cast<array19::Span<int const>>(v.orderedKeys()),
        static_cast<array19::Span<int const>>(array19::Array{12, 23, 42}));
    EXPECT_EQ(
        static_cast<array19::Span<int const>>(v.values()),
        static_cast<array19::Span<int const>>(array19::Array{1, 2, 4}));

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
using array19::spanOfCArray;

TEST(OrderedMapArray, intoMergeBigIntoLittle) {
    using Map = OrderedMapArray<int, int>;
    using KeySpan = OrderedSpan<int const, DefaultLess>;

    // clang-format off
    auto map = Map{
        KeySpan{Array{ 1,  3,  7}},
                Array{10, 11, 12}.begin(),
    };
    map.mergeRange( //
        KeySpan{Array{ 2,  3,  4,  5,  8}},
                Array{20, 21, 22, 23, 24}.begin());

    auto expected = Map{
        KeySpan{Array{ 1,  2,  3,  4,  5,  7,  8}},
                Array{10, 20, 21, 22, 23, 12, 24}.begin(),
    }; //                     ^^ duplicate
    // clang-format on
    EXPECT_EQ(map, expected);
}

TEST(OrderedMapArray, intoMergeLittleIntoBig) {
    using Map = OrderedMapArray<int, int>;
    using KeySpan = OrderedSpan<int const, DefaultLess>;

    // clang-format off
    auto map = Map{
        KeySpan{Array{ 2,  3,  4,  5,  8}},
                Array{20, 21, 22, 23, 24}.begin(),
    };
    map.mergeRange( //
        KeySpan{Array{ 1,  3,  7}},
                Array{10, 11, 12}.begin());

    auto expected = Map{
        KeySpan{Array{ 1,  2,  3,  4,  5,  7,  8}},
                Array{10, 20, 11, 22, 23, 12, 24}.begin(),
    }; //                     ^^ duplicate
    // clang-format on
    EXPECT_EQ(map, expected);
}

TEST(OrderedMapArray, inplaceMergeBigIntoLittle) {
    using Map = OrderedMapArray<int, int>;
    using KeySpan = OrderedSpan<int const, DefaultLess>;

    auto map = Map{};
    // clang-format off
    map.mergeRange( //
        KeySpan{Array{ 1,  3,  7}},
                Array{10, 11, 12}.begin());
    map.ensureUnusedCapacity(5);

    map.mergeRange( //
        KeySpan{Array{ 2,  3,  4,  5,  8}},
                Array{20, 21, 22, 23, 24}.begin());

    auto expected = Map{
        KeySpan{Array{ 1,  2,  3,  4,  5,  7,  8}},
                Array{10, 20, 21, 22, 23, 12, 24}.begin(),
    }; //                     ^^ duplicate
    // clang-format on
    EXPECT_EQ(map, expected);
}

TEST(OrderedMapArray, inplaceMergeLittleIntoBig) {
    using Map = OrderedMapArray<int, int>;
    using KeySpan = OrderedSpan<int const, DefaultLess>;

    auto map = Map{};
    // clang-format off
    map.mergeRange( //
        KeySpan{Array{ 2,  3,  4,  5,  8}},
                Array{20, 21, 22, 23, 24}.begin());
    map.ensureUnusedCapacity(5);

    map.mergeRange( //
        KeySpan{Array{ 1,  3,  7}},
                Array{10, 11, 12}.begin());

    auto expected = Map{
        KeySpan{Array{ 1,  2,  3,  4,  5,  7,  8}},
                Array{10, 20, 11, 22, 23, 12, 24}.begin(),
    }; //                     ^^ duplicate
    // clang-format on
    EXPECT_EQ(map, expected);
}

TEST(OrderedMapArray, inplaceMergeAllFront) {
    using Map = OrderedMapArray<int, int>;
    using KeySpan = OrderedSpan<int const, DefaultLess>;

    auto map = Map{};
    // clang-format off
    map.mergeRange( //
        KeySpan{Array{ 8, 11, 13, 17}},
                Array{ 9, 10, 11, 12}.begin());
    map.ensureUnusedCapacity(5);

    map.mergeRange( //
        KeySpan{Array{ 2,  3,  4,  5,  8}},
                Array{20, 21, 22, 23, 24}.begin());

    auto expected = Map{
        KeySpan{Array{ 2,  3,  4,  5,  8, 11, 13, 17}},
                Array{20, 21, 22, 23, 24, 10, 11, 12}.begin(),
    }; //                             ^^ duplicate
    // clang-format on
    EXPECT_EQ(map, expected);
}

TEST(OrderedMapArray, uniquePtr) {
    using Ptr = std::unique_ptr<int>;
    using Map = OrderedMapArray<int, Ptr>;
    using KeyMoveSpan = OrderedSpan<int&&, DefaultLess>;

    auto a = Map{};
    a.emplace(5, new int{15});

    auto b = std::move(a);
    b.emplace(7, new int{17});

    auto c = Map{};
    auto cValues = Array{Ptr{new int{15}}, Ptr{new int{17}}};
    c.mergeMoveRange(KeyMoveSpan{Array{5, 7}.move()}, cValues.move().begin());

    EXPECT_NE(b, c); // note: compares pointers
}
