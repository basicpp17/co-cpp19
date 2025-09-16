#pragma once
#include "None.h"

/// note: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

template<class... Ts, class Char> struct fmt::formatter<variant19::None, Char> {
    constexpr auto parse(fmt::basic_format_parse_context<Char>& ctx) { return ctx.begin(); }

    template<typename FormatContext> auto format(variant19::None const&, FormatContext& ctx) const {
        return fmt::format_to(ctx.out(), "∅");
    }
};
