#pragma once
#include "array19/Array.h"
#include "serialize.h"

namespace serialize19 {

using array19::Array;

template<Archive A, class T, size_t C> void serialize(A& a, Array<T, C>& array) {
    for (auto& v : array) serialize(a, v);
}

} // namespace serialize19
