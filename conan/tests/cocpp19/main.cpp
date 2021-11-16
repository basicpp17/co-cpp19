#include <coro19/CoEnumerator.range.h>
#include <iostream>
#include <strong19/Strong.h>
#include <strong19/Strong.ostream.h>
#include <strong19/Strong.traits.h>


using namespace coro19;
using namespace strong19;
using meta19::TypePack;

DEFINE_STRONG(Position, int);
DEFINE_STRONG(Distance, int);

auto someValues() -> CoEnumerator<int> {
    co_yield 12;
    co_yield 23;
}

constexpr inline auto operator+(Position p, Distance d) -> Position { return Position{p.v + d.v}; }

int main() {
    auto sum = int{};
    for (auto v : someValues()) sum += v;
    std::cout << "Sum: " << sum << std::endl;

    auto p = Position{};
    auto d = Distance{1};
    [[maybe_unused]] auto p2 = p + d;
    return 0;
}
