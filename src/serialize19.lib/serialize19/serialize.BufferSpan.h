#pragma once
#include "BufferSpan.h"
#include "serialize.h"

namespace serialize19 {

template<Archive A> void serialize(A& a, BufferSpan& span) {
    auto count = static_cast<uint32_t>(span.count());
    serialize(a, count);
    if constexpr (A::mode == ArchiveMode::Read) {
        span = a.withSpan(count);
    }
    else {
        a.withSpan(span);
    }
}

} // namespace serialize19
