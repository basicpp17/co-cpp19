#include "OrderedSetOf.h"

#include <gtest/gtest.h>

using namespace lookup19;

TEST(OrderedSetOf, intExample) {

    auto v = OrderedSetOf<int>{};

    EXPECT_TRUE(v.isEmpty());
    EXPECT_EQ(v.totalCapacity(), 0);
    EXPECT_EQ(v.unusedCapacity(), 0);
    ASSERT_EQ(v.count(), 0);

    v.ensureUnusedCapacity(1);

    EXPECT_GE(v.totalCapacity(), 1);
    EXPECT_GE(v.unusedCapacity(), 1);
    ASSERT_EQ(v.count(), 0);

    v.insert(12);
    v.insert(23);

    ASSERT_EQ(v.count(), 2);
    EXPECT_GE(v.totalCapacity(), 2);

    v.insert(42);
    v.insert(12);

    ASSERT_EQ(v.count(), 3);
    EXPECT_GE(v.totalCapacity(), 3);

    v.remove(OrderedSliceOf{v}.lowerBound(12));

    ASSERT_EQ(v.count(), 2);
}
