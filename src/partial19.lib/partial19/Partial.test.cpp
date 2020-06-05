#include "Partial.equals.h"
#include "Partial.h"

#include <gtest/gtest.h>

using namespace partial19;

using meta19::type;

TEST(Partial, empty) {
    using P = Partial<char, int, float>;
    auto p0 = P{};
    ASSERT_FALSE(p0.which()[0]);
    ASSERT_FALSE(p0.which()[1]);
    ASSERT_FALSE(p0.which()[2]);
}

TEST(Partial, construction) {
    using P = Partial<char, int, float>;
    auto p1 = P::fromArgs('c', 2.3f);
    auto p2 = p1; // copy constructor
    auto p4 = p1; // preparation for move construct
    auto p3 = P{std::move(p4)}; // move construct

    EXPECT_EQ(p1.at<0>(), 'c');
    EXPECT_EQ(p1.at<2>(), 2.3f);
    // EXPECT_EQ(p1, p3);
    // EXPECT_EQ(p1, p2);
    // EXPECT_EQ(p2, p3);
}

TEST(Partial, assignment) {
    using P = Partial<char, int, float>;
    auto p0 = P::fromArgs('c', 2.3f);
    auto p1 = P{};
    p1 = p0; // copy operator
    // EXPECT_EQ(p0, p1);
    auto p2 = P{};
    // EXPECT_EQ(p2, P{});
    p2 = std::move(p1); // move operator

    EXPECT_EQ(p0.at<0>(), 'c');
    EXPECT_EQ(p0.at<2>(), 2.3f);
    // EXPECT_EQ(p0, p2);
}

// TEST(Partial, fromFactory) {
//    const auto p = Partial<char, int, float>::fromFactory(
//        [](size_t i) { return (0 == i % 2); },
//        [](auto i) {
//            if constexpr (i == _index<0>) {
//                return 'a';
//            }
//            if constexpr (i == _index<1>) {
//                return 23;
//            }
//            if constexpr (i == _index<2>) {
//                return 3.14f;
//            }
//        });
//    ASSERT_TRUE(p.which().at<0>());
//    ASSERT_FALSE(p.which().at<1>());
//    ASSERT_TRUE(p.which().at<2>());

//    EXPECT_EQ(p.at<0>(), 'a');
//    EXPECT_EQ(p.at<2>(), 3.14f);
//}

// TEST(Partial, type) {
//    auto p = Partial<char, int, float>{'\x23', 23};
//    ASSERT_TRUE(p.which().of(type<char>));
//    ASSERT_TRUE(p.which()[0]);
//    EXPECT_EQ(0x23, p.of(type<char>));
//    EXPECT_EQ(0x23, p[_index<0>]);
//    ASSERT_TRUE(p.which().of(type<int>));
//    ASSERT_TRUE(p.which()[1]);
//    ASSERT_EQ(23, p.of(type<int>));
//    EXPECT_EQ(23, p[_index<1>]);

//    // TODO CK: Does prevent valid compilation? Microsoft Visual C++ BUG?
//    // auto m = Partial<int, int>{23, 32};
//    // not allowed due to used TypePack
//    // ASSERT_TRUE(m.which().of(type<int>));
//    // ASSERT_EQ(23, m.of(type<int>));
//}

// TEST(Partial, merge) {
//    // Disjunct merge
//    using P = Partial<char, int, float>;
//    auto p0 = P{'\x23'};
//    auto p1 = P{2.3f};
//    auto p2 = p0.merge(p1);
//    ASSERT_TRUE(p2.which().at<0>());
//    ASSERT_FALSE(p2.which().at<1>());
//    ASSERT_TRUE(p2.which().at<2>());

//    EXPECT_EQ(p2.at<0>(), '\x23');
//    EXPECT_EQ(p2.at<2>(), 2.3f);

//    // Overwrite merge
//    auto p3 = P{2};
//    auto p4 = P{4};
//    auto p5 = p3.merge(p4);
//    ASSERT_FALSE(p5.which().at<0>());
//    ASSERT_TRUE(p5.which().at<1>());
//    ASSERT_FALSE(p5.which().at<2>());

//    EXPECT_EQ(p5.at<1>(), 4);

//    auto p6 = p4.merge(p3);
//    ASSERT_FALSE(p6.which().at<0>());
//    ASSERT_TRUE(p6.which().at<1>());
//    ASSERT_FALSE(p6.which().at<2>());

//    EXPECT_EQ(p6.at<1>(), 2);
//}

// TEST(Partial, visitAll) {
//    using P = Partial<char, int, float>;
//    auto p = P{'\x23'};

//    ASSERT_EQ(p.which().count(), 1ul);
//    ASSERT_EQ(p.countAll(), 3ul);

//    size_t count = 0;
//    p.visitAll([&](auto i, auto t) {
//        ++count;
//        if (i == 0) {
//            EXPECT_TRUE(p.which()[i]);
//            EXPECT_EQ(t, type<char>);
//        }
//        else {
//            EXPECT_FALSE(p.which()[i]);
//            EXPECT_NE(t, type<char>);
//        }
//    });
//    EXPECT_EQ(count, p.countAll());
//}

// TEST(Partial, visitInitialized) {
//    using P = Partial<char, int, float>;
//    auto p = P{'\x23', 2.3f};

//    ASSERT_EQ(p.which().count(), 2ul);
//    ASSERT_EQ(p.countAll(), 3ul);

//    size_t count = 0;
//    p.visitInitialized([&](auto v) {
//        ++count;
//        EXPECT_TRUE(type<decltype(v)> == type<char> || type<decltype(v)> == type<float>);
//    });
//    EXPECT_EQ(count, p.which().count());
//}
