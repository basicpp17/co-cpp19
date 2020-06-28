#pragma once
#include "serialize.h"

#include <vector>

namespace serialize19 {

template<Archive A, class T, class... Ts> void serialize(A& a, std::vector<T, Ts...>& vector) {
    auto size = vector.size();
    serialize(a, size);
    if (a.mode == ArchiveMode::Read) {
        vector.resize(size);
    }
    for (auto& value : vector) {
        serialize(a, value);
    }
}

} // namespace serialize19
