#pragma once
#include "DynamicArrayOf.h"

#include <iomanip>
#include <ostream>

namespace array19 {

template<class T> struct AsHex;

template<class T> struct AsHex<DynamicArrayOf<T>> {
    using Array = DynamicArrayOf<T>;
    const Array& a;

    template<class Chr, class Traits>
    friend auto operator<<(std::basic_ostream<Chr, Traits>& out, AsHex<Array>&& a) -> decltype(out)& {
        out << "[";
        bool first = true;
        for (auto& v : a.a) {
            if (first)
                first = false;
            else
                out << ", ";
            out << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << std::internal << +v;
        }
        return out << std::dec << "]";
    }
};

template<class T> AsHex(DynamicArrayOf<T>&&) -> AsHex<DynamicArrayOf<T>>;

} // namespace array19
