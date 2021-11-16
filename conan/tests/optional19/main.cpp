#include <optional19/Optional.equals.h>
#include <optional19/Optional.h>


using namespace optional19;

int main() {
    using OptInt = Optional<int>;

    static_assert(sizeof(OptInt) > sizeof(int));
    {
        [[maybe_unused]] constexpr auto optInt = OptInt{};
        [[maybe_unused]] constexpr const auto constOpt = optInt;
    }
    {
        [[maybe_unused]] constexpr auto optInt = OptInt{23};
        [[maybe_unused]] constexpr const auto constOpt = optInt;

        [[maybe_unused]] auto l = constOpt.map([](int v) { return v * 2; });
    }

    [[maybe_unused]] auto optInt = OptInt{};
    optInt = 23;
    return 0;
}