#pragma once
#include "AllocatedArray.h"
#include "Span.ostream.h"

namespace array19 {

template<class Chr, class Traits, class T>
auto operator<<(std::basic_ostream<Chr, Traits>& out, AllocatedArray<T> const& a) -> decltype(out)& {
    return out << Span{a};
}

} // namespace array19
