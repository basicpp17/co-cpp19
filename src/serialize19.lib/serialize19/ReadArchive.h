#pragma once
#include "ArchiveMode.h"
#include "SliceReader.h"
#include "meta19/Type.h"
#include "serialize.h"

namespace serialize19 {

using meta19::type;

template<EndianBehaviour endian = EndianBehaviour::Keep> struct ReadArchive {
    static constexpr auto mode = ArchiveMode::Read;

    ReadArchive(BufferSlice slice) : m_slice(slice) {}

    auto slice() const -> BufferSlice { return m_slice; }

    template<class T> void withPrimitive(T& value) {
        value = m_slice.as(type<T>);
        m_slice = m_slice.skipOf(type<T>);
    }

private:
    SliceReader<endian> m_slice;
};

} // namespace serialize19
