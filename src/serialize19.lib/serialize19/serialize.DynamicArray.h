#pragma once
#include "array19/DynamicArray.h"
#include "serialize.h"

#include <cstdint> // uint32_t
#include <utility>

namespace serialize19 {

using array19::DynamicArray;

template<Archive A, class T> void serialize(A& a, DynamicArray<T>& dynArray) {
    auto count = static_cast<uint32_t>(dynArray.count());
    serialize(a, count);
    if constexpr (A::mode == ArchiveMode::Read) {
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
