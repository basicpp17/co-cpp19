#pragma once
#include "Tuple.h"

#include <ostream>

namespace tuple19 {

template<class Chr, class Traits, class... Ts>
auto operator<<(std::basic_ostream<Chr, Traits>& out, const Tuple<Ts...>& t) -> decltype(out)& {
    out << "[";
    bool first = true;
    // TODO CK: implement using indices!
    t.visitAll([&](auto& v) {
        if (first)
            first = false;
        else
            out << "; ";
        out << v;
    });
    return out << "]";
}

} // namespace tuple19
