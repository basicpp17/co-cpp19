#pragma once
#include "DynamicRopeOf.h"
#include "array19/SliceOf.ostream.h"
#include "variant19/Variant.ostream.h"

namespace string19 {

/// support for ostream << DynamicRopeOf
/// note: every part of the rope will need ostream support as well!
template<typename Char, typename CharTraits, class... Ts>
auto operator<<(::std::basic_ostream<Char, CharTraits>& out, const DynamicRopeOf<Ts...>& r) -> decltype(auto) {
    return out << "Rope:" << r.slice();
}

} // namespace string19
