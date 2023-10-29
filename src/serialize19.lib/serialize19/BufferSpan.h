#pragma once
#include "array19/Span.h"

#include <stdint.h> // uint8_t

namespace serialize19 {

using array19::Span;
using BufferSpan = Span<uint8_t const>;

} // namespace serialize19
