#pragma once
#include "Archive.h"
#include "SpanReader.h"
#include "meta19/Type.h"
#include "serialize.h"

#include <stddef.h> // size_t

namespace serialize19 {

using meta19::type;

/// Archive Adapter that allows reading from serialize() methods
/// see SpanReader
template<EndianBehaviour endian = EndianBehaviour::Keep> struct ReadArchive {
    static constexpr auto mode = ArchiveMode::Read;

    ReadArchive(BufferSpan span) : m_span(span) {}

    auto span() const -> BufferSpan { return m_span; }

    auto withSpan(size_t n) -> BufferSpan {
        auto r = m_span.sliceBytes(n);
        m_span = m_span.skipBytes(n);
        return r;
    }

    template<class T> void withPrimitive(T& value) {
        value = m_span.as(type<T>);
        m_span = m_span.skipOf(type<T>);
    }

    void withPrimitive(bool& value) {
        value = m_span.as(type<uint8_t>) == 1;
        m_span = m_span.skipOf(type<uint8_t>);
    }

private:
    SpanReader<endian> m_span;
};

static_assert(Archive<ReadArchive<>>);

} // namespace serialize19
