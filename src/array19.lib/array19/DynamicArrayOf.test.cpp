#include "DynamicArrayOf.equals.h"
#include "DynamicArrayOf.h"
#include "DynamicArrayOf.ostream.h"
#include "SliceOf.carray.h"
#include "SliceOf.equals.h"

#include <gtest/gtest.h>

using namespace array19;

TEST(DynamicArrayOf, intExample) {

    auto v = DynamicArrayOf<int>{};

    EXPECT_TRUE(v.isEmpty());
    EXPECT_EQ(v.totalCapacity(), 0);
    EXPECT_EQ(v.unusedCapacity(), 0);
    ASSERT_EQ(v.count(), 0);

    v.ensureUnusedCapacity(1);

    EXPECT_GE(v.totalCapacity(), 1);
    EXPECT_GE(v.unusedCapacity(), 1);
    ASSERT_EQ(v.count(), 0);

    v.append(sliceOfCArray({12, 23}));

    ASSERT_EQ(v.count(), 2);
    EXPECT_GE(v.totalCapacity(), 2);

    v.append(sliceOfCArray({42, 64}));

    ASSERT_EQ(v.count(), 4);

    v.splice(v.amendBegin() + 1, 2, sliceOfCArray({99}));

    auto v2 = DynamicArrayOf<int>{};
    v2.append(sliceOfCArray({12, 99, 64}));
    ASSERT_EQ(v, v2);

    v2.append(sliceOfCArray({45}));
    v2 = v; // copy
    ASSERT_EQ(v, v2);

    v.append(sliceOfCArray({45}));
    v2 = std::move(v);
    ASSERT_EQ(sliceOfCArray({12, 99, 64, 45}), SliceOf{v2});
}

TEST(DynamicArrayOf, construct) {
    auto v = DynamicArrayOf{1, 2, 3};
    ASSERT_EQ(sliceOfCArray({1, 2, 3}), SliceOf{v});
}

struct NonTrivial;
using NonTrivialArray = DynamicArrayOf<NonTrivial>;

struct User {
    NonTrivialArray v;

    User() = default;
};

struct NonTrivial {
    NonTrivial() : v(1) {}
    explicit NonTrivial(int v) : v(v) {}
    NonTrivial(const NonTrivial& o) : v(o.v) {}
    NonTrivial& operator=(const NonTrivial& o) { return v = o.v, *this; }
    NonTrivial(NonTrivial&& o) : v(o.v) {}
    NonTrivial& operator=(NonTrivial&& o) { return v = o.v, *this; }
    ~NonTrivial() {}
    bool operator==(const NonTrivial&) const = default;

    int value() const { return v; }

private:
    int v;
};
auto operator<<(std::ostream& out, const NonTrivial& c) -> std::ostream& { return out << c.value(); }

TEST(DynamicArrayOf, NontrivialExample) {

    auto v = DynamicArrayOf<NonTrivial>{};

    EXPECT_TRUE(v.isEmpty());
    EXPECT_EQ(v.totalCapacity(), 0);
    EXPECT_EQ(v.unusedCapacity(), 0);
    ASSERT_EQ(v.count(), 0);

    v.ensureUnusedCapacity(1);

    EXPECT_GE(v.totalCapacity(), 1);
    EXPECT_GE(v.unusedCapacity(), 1);
    ASSERT_EQ(v.count(), 0);

    v.append(sliceOfCArray({NonTrivial{12}, NonTrivial{23}}));

    ASSERT_EQ(v.count(), 2);
    EXPECT_GE(v.totalCapacity(), 2);

    v.append(sliceOfCArray({NonTrivial{42}, NonTrivial{64}}));

    ASSERT_EQ(v.count(), 4);

    v.splice(v.amendBegin() + 1, 2, sliceOfCArray({NonTrivial{99}}));

    auto v2 = DynamicArrayOf<NonTrivial>{};
    v2.append(sliceOfCArray({NonTrivial{12}, NonTrivial{99}, NonTrivial{64}}));
    ASSERT_EQ(v, v2);
}
