#include "AllocatedArray.h"

#include "AllocatedArray.equals.h"
#include "AllocatedArray.ostream.h"
#include "Span.carray.h"
#include "Span.equals.h"
#include "Span.ostream.h"

#include <gtest/gtest.h>
#include <utility>

using namespace array19;

TEST(AllocatedArray, construct) {
    auto v = AllocatedArray{1, 2, 3};
    ASSERT_EQ(spanOfCArray({1, 2, 3}), Span{v});
}

namespace {

struct NonTrivial;
using NonTrivialArray = AllocatedArray<NonTrivial>;

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
    ~NonTrivial() noexcept { c = false; }
    bool operator==(const NonTrivial&) const = default;

    int value() const { return v; }

private:
    bool c{true};
    int v;
};

[[maybe_unused]] auto operator<<(std::ostream& out, const NonTrivial& c) -> std::ostream& { return out << c.value(); }

} // namespace

TEST(AllocatedArray, NontrivialExample) {
    auto v = AllocatedArray<NonTrivial>{};

    EXPECT_TRUE(v.isEmpty());
    ASSERT_EQ(v.count(), 0u);

    v = AllocatedArray{NonTrivial{}, NonTrivial{2}};

    ASSERT_EQ(v.count(), 2u);

    auto v2 = v;
    ASSERT_EQ(v, v2);
}

TEST(AllocatedArray, MoveAssignAfterMove) {
    using E = NonTrivial;
    using AA = AllocatedArray<E>;
    auto v = AA{E{1}};

    auto v2 = std::move(v);
    ASSERT_EQ(v2, (AA{E{1}}));

    v = AA{E{2}, E{3}};
    ASSERT_EQ(v, (AA{E{2}, E{3}}));
}

TEST(AllocatedArray, CopyAssignAfterMove) {
    using E = NonTrivial;
    using AA = AllocatedArray<E>;
    auto v = AA{E{1}};

    auto v2 = std::move(v);
    ASSERT_EQ(v2, (AA{E{1}}));

    auto v3 = AA{E{2}, E{3}};
    v = v3;
    ASSERT_EQ(v, (AA{E{2}, E{3}}));
}
