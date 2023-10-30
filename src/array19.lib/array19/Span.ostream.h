#pragma once
#include "Span.h"

#include <ostream>

namespace array19 {

template<class Chr, class Traits, class T>
auto operator<<(std::basic_ostream<Chr, Traits>& out, Span<T> const& s) -> decltype(out)& {
    out << "[";
    bool first = true;
    for (auto& v : s) {
        if (first)
            first = false;
        else
            out << ", ";
        out << v;
    }
    return out << "]";
}

} // namespace array19
