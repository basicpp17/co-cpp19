#pragma once
#include "Archive.h"
#include "UniqueBuffer.h"

namespace serialize19 {

/// serializes T to a dynamically allocated UniqueBuffer
template<EndianBehaviour endian = EndianBehaviour::Keep, class T> auto dynamicWrite(const T& value) -> UniqueBuffer {
    auto sizeArchive = SizeArchive{};
    serialize(sizeArchive, value);
    auto buffer = UniqueBuffer{sizeArchive.size()};
    auto writeArchive = WriteToArchive<endian>{buffer.amendSlice()};
    serialize(writeArchive, value);
    return buffer;
}

} // namespace serialize19
