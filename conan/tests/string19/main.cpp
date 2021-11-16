#include <string19/Rope.StringStore.h>
#include <string19/Rope.StringView.h>
#include <string19/Rope.char.h>
#include <string19/Rope.store.h>
#include <string19/StringStore.literal.h>
#include <string19/StringView.equals.h>
#include <string19/StringView.literal.h>
#include <string19/StringView.store.h>

using namespace string19;

int main() {
    constexpr auto world = viewLiteral("World");
    constexpr auto stored = storeLiteral("stored");

    constexpr auto rope = Rope{viewLiteral("Hello"), ' ', stored, ' ', world};
    constexpr auto N = ropeCount(rope);
    constexpr auto store = ropeStore<N>(rope);
    static_assert(viewStore(store) == viewLiteral("Hello stored World"));
    return 0;
}