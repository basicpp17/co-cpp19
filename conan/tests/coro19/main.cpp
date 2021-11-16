#include <coro19/CoEnumerator.range.h>

#include <iostream>

using namespace coro19;

auto someValues() -> CoEnumerator<int> {
    co_yield 12;
    co_yield 23;
}

int main() {
    auto sum = int{};
    for (auto v : someValues()) sum += v;
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}
