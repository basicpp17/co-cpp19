#include <partial19/Partial.equals.h>
#include <partial19/Partial.h>

#include <iostream>

using namespace partial19;

int main() {
    using P = Partial<char, int, float>;
    auto p0 = P{};
    std::cout << std::boolalpha << p0.which()[0] << std::endl;
    std::cout << std::boolalpha << p0.which()[1] << std::endl;
    std::cout << std::boolalpha << p0.which()[2] << std::endl;
    return 0;
}