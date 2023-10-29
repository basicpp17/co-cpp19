#pragma once
#include "BufferSpan.h"

#include <stddef.h> // size_t

namespace serialize19 {

/// UniqueBuffer is allocated on construction and carries the ownership of the memory
/// Use spans to access the data
struct UniqueBuffer {
    using ConstSpan = BufferSpan;
    using AmendSpan = Span<uint8_t>;

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

    [[nodiscard]] auto size() const -> size_t { return m_size; }
    [[nodiscard]] auto span() const -> ConstSpan { return ConstSpan{m_pointer, m_size}; }
    [[nodiscard]] auto amend() -> AmendSpan { return AmendSpan{m_pointer, m_size}; }

private:
    uint8_t* m_pointer{};
    size_t m_size{};
};

} // namespace serialize19
