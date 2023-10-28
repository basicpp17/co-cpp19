#pragma once
#include "OrderedMapOf.h"
#include "array19/SliceOf.equals.h"

namespace lookup19 {

template<class Key, class Value, class Less>
bool operator==(const OrderedMapOf<Key, Value, Less>& a, const OrderedMapOf<Key, Value, Less>& b) {
    return a.keys() == b.keys() && a.values() == b.values();
}

} // namespace lookup19
