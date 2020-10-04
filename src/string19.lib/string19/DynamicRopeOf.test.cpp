#include "DynamicRopeOf.h"
#include "Rope.DynamicString.h"
#include "Rope.StringView.h"
#include "Rope.char.h"
#include "StringView.literal.h"

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
}
