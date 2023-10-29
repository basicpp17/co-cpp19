#include <array19/DynamicArray.equals.h>
#include <array19/DynamicArray.ostream.h>
#include <gtest/gtest.h>
#include <serialize19/ReadArchive.h>
#include <serialize19/dynamicWrite.h>
#include <serialize19/serialize.DynamicArray.h>

using namespace serialize19;

TEST(serialize, DynamicArray) {
    using T = array19::DynamicArray<int>;
    auto input = T{7, 13, 23};

    auto buffer = dynamicWrite(input);

    auto reader = ReadArchive{buffer.span()};
    auto output = T{};
    serialize(reader, output);

    EXPECT_EQ(output, input);
}

TEST(serialize, DynamicArrayOf_empty) {
    using T = array19::DynamicArray<int>;
    auto input = T{};

    auto buffer = dynamicWrite(input);

    auto reader = ReadArchive{buffer.span()};
    auto output = T{};
    serialize(reader, output);

    EXPECT_EQ(output, input);
}
