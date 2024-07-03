#include "FlagsOf.h"

#include "FlagsOf.ostream.h"

#include <gtest/gtest.h>

using namespace flags19;

namespace my_test {

ENUM19(CheckBit, uint8_t, Ordered, Packaged, Delivered, Received, Complained, ReturnLabeled, ReturnReceived);
using CheckBits = FlagsOf<CheckBit>;

} // namespace my_test

TEST(Flags, example) {
    using enum my_test::CheckBit;
    constexpr auto checks = my_test::CheckBits{Ordered, Delivered};

    static_assert(checks[Ordered]);
    static_assert(checks[Delivered]);
    static_assert(!checks[Packaged]);
    static_assert(!checks[ReturnReceived]);

    static_assert(checks.allOf(Delivered));
    static_assert(!checks.allOf(Delivered, Received));

    static_assert(checks.someOf(Delivered, Received));

    static_assert(checks.noneOf(Complained, Received));

    auto mutChecks = checks;
    mutChecks.set(Packaged);
    EXPECT_TRUE(mutChecks[Packaged]);

    mutChecks.reset(Delivered, Received);
    EXPECT_FALSE(mutChecks[Delivered]);
    EXPECT_TRUE(mutChecks.noneOf(Delivered, Received));

    mutChecks.toggle(Packaged, Delivered);
    EXPECT_EQ(mutChecks, checks);
}
