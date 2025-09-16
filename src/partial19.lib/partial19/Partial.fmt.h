#pragma once
#include "Partial.h"

/// note: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

template<class... Ts, class Char> struct fmt::formatter<partial19::Partial<Ts...>, Char> {
    constexpr auto parse(fmt::basic_format_parse_context<Char>& ctx) { return ctx.begin(); }

    template<class FormatContext> auto format(partial19::Partial<Ts...> const& t, FormatContext& ctx) const {
        bool first = true;
        auto out = ctx.out();
        t.visitInitialized([&](auto& v) {
            if (first) {
                out = fmt::format_to(out, "<[{}", v);
                first = false;
            }
            else {
                out = fmt::format_to(out, "; {}", v);
            }
        });
        return fmt::format_to(out, "]>");
    }
};
