#pragma once
#include "Variant.h"

/// note: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

template<class... Ts, class Char> struct fmt::formatter<variant19::Variant<Ts...>, Char> {
    constexpr auto parse(fmt::basic_format_parse_context<Char>& ctx) { return ctx.begin(); }

    template<class FormatContext> auto format(variant19::Variant<Ts...> const& v, FormatContext& ctx) const {
        if constexpr (sizeof...(Ts) == 0) {
            return fmt::format_to(ctx.out(), "Variant()");
        }
        else {
            return v.visit([&](auto const& elem) { return fmt::format_to(ctx.out(), "Variant({})", elem); });
        }
    }
};
