#pragma once
#include "Enum.names.h"
#include "string19/StringView.fmt.h"

/// noto: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

namespace fmt {

/// adds strong support for fmt
template<enum19::HasMetaEnum T, class char_type> struct formatter<T, char_type> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatCtx> auto format(const T& v, FormatCtx& ctx) {
        auto underlying = static_cast<std::underlying_type_t<T>>(v);
        return format_to(ctx.out(), "{} ({})", valueName(v), underlying);
    }
};

} // namespace fmt
