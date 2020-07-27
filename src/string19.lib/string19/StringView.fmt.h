#pragma once
#include "StringView.h"

/// note: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

namespace fmt {

/// adds strong support for fmt
template<class char_type> struct formatter<string19::StringView, char_type> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatCtx> auto format(const string19::StringView& v, FormatCtx& ctx) {
        auto out = ctx.begin();
        for (auto i = 0u; i < v.count; i++) *out++ = v[i];
        return out;
    }
};

} // namespace fmt
