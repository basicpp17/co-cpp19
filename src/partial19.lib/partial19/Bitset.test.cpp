#include "Bitset.h"

#include <gtest/gtest.h>

using namespace partial19;

TEST(Bitset, example10) {
    using Bits10 = Bitset<10>;
    static_assert(Bits10::count == 10);

    auto bits10 = Bits10{};
    bits10.setAt(0);
    EXPECT_TRUE(bits10.at(0));

    bits10.setAt(9);

    EXPECT_TRUE(bits10.at(9));
    EXPECT_FALSE(bits10.at(8));

    bits10.resetAt(9);
    EXPECT_FALSE(bits10.at(9));

    bits10.reset();
    EXPECT_FALSE(bits10.at(0));
}

TEST(Bitset, example99) {
    using Bits99 = Bitset<99>;
    static_assert(Bits99::count == 99);

    auto bits99 = Bits99{};
    bits99.setAt(0);
    EXPECT_TRUE(bits99.at(0));

    bits99.setAt(98);
    EXPECT_TRUE(bits99.at(98));
    EXPECT_FALSE(bits99.at(97));

    bits99.resetAt(98);
    EXPECT_FALSE(bits99.at(98));

    bits99.reset();
    EXPECT_FALSE(bits99.at(0));
}
