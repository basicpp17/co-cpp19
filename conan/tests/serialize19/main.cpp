#include <serialize19/ReadArchive.h>
#include <serialize19/dynamicWrite.h>
#include <serialize19/serialize.std_string.h>
#include <serialize19/serialize.std_tuple.h>

using namespace serialize19;

int main() {
    using T = std::tuple<int, float, std::string>;
    auto input = T{23, 3.14f, std::string("glurbz")};

    auto buffer = dynamicWrite(input);

    auto reader = ReadArchive{buffer.slice()};
    auto output = T{};
    serialize(reader, output);

}
