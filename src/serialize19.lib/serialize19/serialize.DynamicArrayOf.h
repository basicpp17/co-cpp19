#pragma once
#include "array19/DynamicArrayOf.h"
#include "serialize.h"

#include <stdint.h> // uint32_t

namespace serialize19 {

using array19::DynamicArrayOf;

template<Archive A, class T> void serialize(A& a, DynamicArrayOf<T>& dynArray) {
    auto count = static_cast<uint32_t>(dynArray.count());
    serialize(a, count);
    if (a.mode == ArchiveMode::Read) {
        dynArray.ensureCapacity(count);
        for (auto i = 0U; i < count; i++) {
            auto value = T{};
            serialize(a, value);
            dynArray.emplace_back(std::move(value));
        }
    }
    else {
        for (auto& value : dynArray) serialize(a, value);
    }
}

} // namespace serialize19
