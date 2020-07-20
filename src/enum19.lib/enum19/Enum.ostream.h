#pragma once
#include "Enum.names.h"

#include <ostream>
#include <string19/StringView.ostream.h>

namespace enum19 {

/// This wrapper enables ADL for ostream operator
template<class T> requires(std::is_enum_v<T>) struct EnumName { T v; };

template<class T> EnumName(T) -> EnumName<T>;

template<class T> auto operator<<(std::ostream& out, EnumName<T> wrapper) -> std::ostream& {
    auto underlying = static_cast<std::underlying_type_t<T>>(wrapper.v);
    return out << valueName(wrapper.v) << " (" << underlying << ")";
}

} // namespace enum19
