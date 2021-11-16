#include <strong19/Strong.h>
#include <strong19/Strong.ostream.h>
#include <strong19/Strong.traits.h>


using namespace strong19;
using meta19::TypePack;

DEFINE_STRONG(Position, int);
DEFINE_STRONG(Distance, int);

constexpr inline auto operator+(Position p, Distance d) -> Position { return Position{p.v + d.v}; }

int main() {
    auto p = Position{};
    auto d = Distance{1};
    [[maybe_unused]] auto p2 = p + d;

    return 0;
}