#include "Tuple.h"

#include "Tuple.join.h"
#include "Tuple.ostream.h"

#include <gtest/gtest.h>
#include <string>

using namespace tuple19;

TEST(TupleJoin, constants) {
    using meta19::_index;
    using meta19::type;

    using A = Tuple<char, int, double>;
    auto a = A{'x', 23, 4.2};

    using B = Tuple<std::string, float, int>;
    auto b = B{"!", 4.2f, 42};

    using C = Tuple<char, int, double, std::string, float, int>;
    auto c = C{'x', 23, 4.2, "!", 4.2f, 42};

    EXPECT_EQ(joinTuples(a, b), c);
}

TEST(TupleJoin, move) {
    //
    EXPECT_EQ(joinTuples(Tuple{'a', 12}, Tuple{4.2, 4.2f, 23}), (Tuple{'a', 12, 4.2, 4.2f, 23}));
}
