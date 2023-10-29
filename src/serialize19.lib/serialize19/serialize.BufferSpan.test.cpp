#include <array19/Span.equals.h>
#include <array19/Span.ostream.h>
#include <gtest/gtest.h>
#include <serialize19/ReadArchive.h>
#include <serialize19/dynamicWrite.h>
#include <serialize19/serialize.BufferSpan.h>

using namespace serialize19;

TEST(serialize, BufferSpan) {
    uint8_t data[] = "Hello";
    using T = BufferSpan;
    auto input = T{data, sizeof(data)};

    auto buffer = dynamicWrite(input);

    EXPECT_EQ(buffer.size(), sizeof(data) + sizeof(uint32_t));

    auto reader = ReadArchive{buffer.span()};
    auto output = T{};
    serialize(reader, output);

    EXPECT_EQ(output, input);
}
