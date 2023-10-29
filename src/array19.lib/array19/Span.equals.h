#pragma once
#include "Span.h"

#include <stddef.h> // size_t

namespace array19 {

template<class T> constexpr bool operator==(Span<T> const& a, Span<T> const& b) {
    if (a.count() != b.count()) {
        return false;
    }
    for (auto i = size_t{}; i < a.count() && i < b.count(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// generated automatilly now
// template<class T> constexpr bool operator!=(Span<T> const& a, Span<T> const& b) { return !(a == b); }

} // namespace array19
