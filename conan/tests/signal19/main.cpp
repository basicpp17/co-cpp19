#include <signal19/SignalWith.h>

using namespace signal19;

int main() {
    using S = SignalWith<int>;
    auto s = S{};

    s.emit(7);

    auto r = 0u;
    auto h = [&](int v) { r += v; };
    {
        auto c = s.subscribe(h);
        auto s2 = S{};
        s.emit(13);
    }
    s.emit(17);
    return 0;
}