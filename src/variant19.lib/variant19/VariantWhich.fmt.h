#pragma once
#include "Variant.h"

/// note: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

/// note: for this header you need strong19 (not included as library dependency)
#include <strong19/Strong.h>

template<class... Ts, class Char> struct fmt::formatter<variant19::VariantWhich<Ts...>, Char> {
    constexpr auto parse(fmt::basic_format_parse_context<Char>& ctx) { return ctx.begin(); }

    template<size_t I, class T> static auto format_type_index(size_t i, auto& out) -> bool {
        if (i == I) {
            if constexpr (strong19::is_strong<T>) {
                out = fmt::format_to(out, "Which<{}>({})", strong19::strong_name<T>, i);
            }
            else {
                out = fmt::format_to(out, "Which({})", i);
            }
        }
        return (i == I);
    }

    template<typename FormatContext> auto format(variant19::VariantWhich<Ts...> const& v, FormatContext& ctx) const {
        if constexpr (sizeof...(Ts) == 0) {
            return fmt::format_to(ctx.out(), "Which()");
        }
        else {
            return [&]<size_t... Is>(std::index_sequence<Is...> const&) {
                auto out = ctx.out();
                ((format_type_index<Is, Ts>(v, out)) || ... ||
                 (out = fmt::format_to(out, "Which({})", static_cast<size_t>(v)), true));
                return out;
            }(std::make_index_sequence<sizeof...(Ts)>{});
        }
    }
};
