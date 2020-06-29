#include "ReadArchive.h"
#include "dynamicWrite.h"
#include "serialize.BufferSlice.h"

#include <array19/SliceOf.equals.h>
#include <array19/SliceOf.ostream.h>
#include <gtest/gtest.h>

using namespace serialize19;

TEST(serialize, BufferSlice) {
    uint8_t data[] = "Hello";
    using T = BufferSlice;
    auto input = T{data, sizeof(data)};

    auto buffer = dynamicWrite(input);

    EXPECT_EQ(buffer.size(), sizeof(data) + sizeof(uint32_t));

    auto reader = ReadArchive{buffer.slice()};
    auto output = T{};
    serialize(reader, output);

    EXPECT_EQ(output, input);
}
