#include "Rope.StringView.h"
#include "Rope.char.h"
#include "Rope.join.h"
#include "Rope.store.h"
#include "StringStore.literal.h"
#include "StringView.equals.h"
#include "StringView.literal.h"
#include "StringView.store.h"

#include <gtest/gtest.h>

using namespace string19;

TEST(Rope, joinChar) {
    constexpr auto rope = Seperator{','}.join('1', '2', '3');
    constexpr auto N = ropeCount(rope);
    constexpr auto store = ropeStore<N>(rope);
    static_assert(viewStore(store) == viewLiteral("1,2,3"));
    EXPECT_EQ(store, storeLiteral("1,2,3"));
}

TEST(Rope, joinView) {
    constexpr auto rope = Seperator{viewLiteral(", ")}.join('1', '2', '3');
    constexpr auto N = ropeCount(rope);
    constexpr auto store = ropeStore<N>(rope);
    static_assert(viewStore(store) == viewLiteral("1, 2, 3"));
    EXPECT_EQ(store, storeLiteral("1, 2, 3"));
}
