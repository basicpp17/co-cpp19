#pragma once
#include "Strong.h"
#include "string19/StringView.fmt.h"

/// noto: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

namespace fmt {

/// adds strong support for fmt
template<class T, class char_type> requires(strong19::is_strong<T>) struct formatter<T, char_type> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatCtx> auto format(const T& v, FormatCtx& ctx) {
        return format_to(ctx.out(), "{} [{}]", v.v, strong19::strong_name<T>);
    }
};

} // namespace fmt
