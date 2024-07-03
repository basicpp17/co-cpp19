#pragma once
#include "FlagsOf.h"

#include <bitset>
#include <enum19/Enum.names.h>
#include <iosfwd>
#include <string19/StringView.ostream.h>

namespace flags19 {

template<class Enum> auto operator<<(std::ostream& out, FlagsOf<Enum> const& flags) -> std::ostream& {
    using namespace std::string_view_literals;
    auto printed = false;
    for (auto& member : enum19::meta_enum_for<Enum>.members) {
        if (!flags[member.value]) continue;
        out << (printed ? "|"sv : ""sv) << enum19::valueName(member.value);
        printed = true;
    }
    return out << (printed ? " ("sv : "<> ("sv)
               << std::bitset<max_underlying_value_of<Enum>>{static_cast<typename FlagsOf<Enum>::Value>(flags)} << ')';
}

} // namespace flags19
