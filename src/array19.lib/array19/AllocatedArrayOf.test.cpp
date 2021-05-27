#include "AllocatedArrayOf.equals.h"
#include "AllocatedArrayOf.h"
#include "AllocatedArrayOf.ostream.h"
#include "SliceOf.carray.h"
#include "SliceOf.equals.h"
#include "SliceOf.ostream.h"

#include <gtest/gtest.h>

using namespace array19;

TEST(AllocatedArrayOf, construct) {
    auto v = AllocatedArrayOf{1, 2, 3};
    ASSERT_EQ(sliceOfCArray({1, 2, 3}), SliceOf{v});
}

namespace {

struct NonTrivial;
using NonTrivialArray = AllocatedArrayOf<NonTrivial>;

struct User {
    NonTrivialArray v;

    User() = default;
};

struct NonTrivial {
    NonTrivial() : v(1) {}
    explicit NonTrivial(int v) : v(v) {}
    NonTrivial(const NonTrivial& o) : v(o.v) {}
    NonTrivial& operator=(const NonTrivial& o) { return v = o.v, *this; }
    NonTrivial(NonTrivial&& o) noexcept : v(o.v) {}
    NonTrivial& operator=(NonTrivial&& o) noexcept { return v = o.v, *this; }
    ~NonTrivial() noexcept {}
    bool operator==(const NonTrivial&) const = default;

    int value() const { return v; }

private:
    int v;
};

[[maybe_unused]] auto operator<<(std::ostream& out, const NonTrivial& c) -> std::ostream& { return out << c.value(); }

} // namespace

TEST(AllocatedArrayOf, NontrivialExample) {
    auto v = AllocatedArrayOf<NonTrivial>{};

    EXPECT_TRUE(v.isEmpty());
    ASSERT_EQ(v.count(), 0u);

    v = AllocatedArrayOf{NonTrivial{}, NonTrivial{2}};

    ASSERT_EQ(v.count(), 2u);

    auto v2 = v;
    ASSERT_EQ(v, v2);
}
