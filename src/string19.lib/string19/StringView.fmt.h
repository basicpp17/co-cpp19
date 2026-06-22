#pragma once
#include "StringView.h"

/// note: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h> // oder <fmt/base.h>
#include <fmt/ranges.h> // muss vor deiner Spezialisierung sein

namespace fmt {

template<typename Char> struct range_format_kind<string19::StringView, Char> {
    static constexpr auto value = range_format::disabled;
};

} // namespace fmt

template<class Char> struct fmt::formatter<string19::StringView, Char> {
    constexpr auto parse(fmt::basic_format_parse_context<Char>& ctx) { return ctx.begin(); }

    template<typename FormatContext> auto format(const string19::StringView& v, FormatContext& ctx) const {
        auto out = ctx.out();
        for (auto i = 0u; i < v.count; i++) *out++ = v[i];
        return out;
    }
};
