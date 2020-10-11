#include "DynamicRopeOf.equals.h"
#include "DynamicRopeOf.h"
#include "DynamicRopeOf.ostream.h"
#include "Rope.DynamicString.h"
#include "Rope.StringView.h"
#include "Rope.char.h"
#include "StringView.equals.h"
#include "StringView.literal.h"
#include "StringView.ostream.h"

#include <gtest/gtest.h>

using namespace string19;

auto basic() {
    using Rope = DynamicRopeOf<char, StringView>;
    auto r = Rope{};
    r += viewLiteral("Hello");
    r += ' ';
    return r;
}

TEST(DynamicRopeOf, example) {
    using Rope = DynamicRopeOf<DynamicString, char, StringView>;
    auto r = Rope{};
    r += basic();
    r += DynamicString("World");
    r += '!';
    EXPECT_EQ(r.build(), DynamicString("Hello World!"));

    auto r2 = r;
    // r2 += '!';
    EXPECT_EQ(r, r2);
}
