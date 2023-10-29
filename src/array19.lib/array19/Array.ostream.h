#pragma once
#include "Array.h"

#include <ostream>
#include <stddef.h> // size_t

namespace array19 {

template<class Chr, class Traits, class T, size_t N>
auto operator<<(std::basic_ostream<Chr, Traits>& out, Array<T, N> const& a) -> decltype(out)& {
    out << "[";
    bool first = true;
    for (auto& v : a) {
        if (first)
            first = false;
        else
            out << ", ";
        out << v;
    }
    return out << "]";
}

} // namespace array19
