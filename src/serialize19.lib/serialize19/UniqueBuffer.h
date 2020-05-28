#pragma once
#include "array19/SliceOf.h"

#include <stdint.h> // uint8_t

namespace serialize19 {

using array19::SliceOf;

/// UniqueBuffer is allocated on construction and carries the ownership of the memory
/// Use slices to access the data
struct UniqueBuffer {
    using Size = size_t;
    using Index = size_t;
    using Slice = SliceOf<uint8_t>;
    using ConstSlice = SliceOf<const uint8_t>;

    UniqueBuffer() = default;
    explicit UniqueBuffer(size_t size) : m_pointer(new uint8_t[size]), m_size(size) {}
    ~UniqueBuffer() { delete[] m_pointer; }

    UniqueBuffer(UniqueBuffer&& o) noexcept : m_pointer(o.m_pointer), m_size(o.m_size) { o.m_pointer = {}; }
    UniqueBuffer& operator=(UniqueBuffer&& o) noexcept {
        delete[] m_pointer;
        m_pointer = o.m_pointer;
        m_size = o.m_size;
        o.m_pointer = {};
        return *this;
    }

    [[nodiscard]] auto size() const -> Size { return m_size; }

    [[nodiscard]] auto slice() const -> ConstSlice { return ConstSlice{m_pointer, m_pointer + m_size}; }

    [[nodiscard]] auto amendSlice() -> Slice { return Slice{m_pointer, m_pointer + m_size}; }

private:
    uint8_t* m_pointer{};
    Size m_size{};
};

} // namespace serialize19
