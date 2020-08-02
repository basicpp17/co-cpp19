#pragma once
#include "StringStore.h"

#include <ostream>

namespace string19 {

template<size_t N> inline auto operator<<(std::ostream& out, const StringStore<N>& v) -> std::ostream& {
    return out.write(v.data, v.count);
}

} // namespace string19
