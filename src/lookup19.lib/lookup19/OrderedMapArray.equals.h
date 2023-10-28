#pragma once
#include "OrderedMapArray.h"
#include "array19/Span.equals.h"

namespace lookup19 {

template<class Key, class Value, class Less>
bool operator==(const OrderedMapArray<Key, Value, Less>& a, const OrderedMapArray<Key, Value, Less>& b) {
    return a.keys() == b.keys() && a.values() == b.values();
}

} // namespace lookup19
