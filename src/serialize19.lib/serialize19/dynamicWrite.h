#pragma once
#include "SizeArchive.h"
#include "UniqueBuffer.h"
#include "WriteToArchive.h"

namespace serialize19 {

/// serializes T to a dynamically allocated UniqueBuffer
template<EndianBehaviour endian = EndianBehaviour::Keep, class T, class... Ts>
auto dynamicWrite(const T& value, const Ts&... values) -> UniqueBuffer {
    auto sizeArchive = SizeArchive{};
    serialize(sizeArchive, value);
    (serialize(sizeArchive, values), ...);
    auto buffer = UniqueBuffer{sizeArchive.size()};
    auto writeArchive = WriteToArchive<endian>{buffer.amendSlice()};
    serialize(writeArchive, value);
    (serialize(writeArchive, values), ...);
    return buffer;
}

} // namespace serialize19
