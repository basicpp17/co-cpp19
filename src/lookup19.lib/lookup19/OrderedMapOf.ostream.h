#pragma once
#include "OrderedMapOf.h"
#include "array19/Zip.h"

#include <ostream>

namespace lookup19 {

template<class Chr, class Traits, class Key, class Value, class Less>
auto operator<<(std::basic_ostream<Chr, Traits>& out, OrderedMapOf<Key, Value, Less> const& s) -> decltype(out)& {
    out << "{";
    bool first = true;
    for (auto [key, value] : array19::Zip{s.keys(), s.values()}) {
        if (first)
            first = false;
        else
            out << ", ";
        out << key << " => " << value;
    }
    return out << "}";
}

} // namespace lookup19
