#include <variant19/Variant.equals.h>
#include <variant19/Variant.h>

using namespace variant19;

int main() {
    [[maybe_unused]] auto v = Variant<char, int, float>{};

    [[maybe_unused]] auto size0 = v.visit([](auto value) { return sizeof(value); });

    v = 3.2f;

    [[maybe_unused]] auto size1 = v.visit([](auto value) { return sizeof(value); });
    return 0;
}