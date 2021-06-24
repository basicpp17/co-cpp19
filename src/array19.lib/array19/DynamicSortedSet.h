#pragma once
#include "DynamicArrayOf.h"
#include "SliceOf.single.h"

#include <algorithm>

namespace array19 {

/// set of sorted of values
/// - useful for sets of ids
template<class T> struct DynamicSortedSet {
    auto count() const -> size_t { return m.count(); }
    auto begin() const -> const T* { return m.begin(); }
    auto end() const -> const T* { return m.end(); }

    bool has(const T& v) const {
        auto [b, e] = std::equal_range(m.begin(), m.end(), v);
        return b != e;
    }

    void add(const T& v) {
        auto [b, e] = std::equal_range(m.amendBegin(), m.amendEnd(), v);
        if (b == e) {
            m.splice(b, 0, array19::sliceOfSingle(v));
        }
    }

    void remove(const T& v) {
        auto [b, e] = std::equal_range(m.amendBegin(), m.amendEnd(), v);
        if (b != e) {
            m.remove(b, 1);
        }
    }

    void clear() { m.clear(); }

private:
    DynamicArrayOf<T> m;
};

} // namespace array19
