#pragma once
#include "Partial.h"

/// note: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

/// note: for this header you need strong19 (not included as library dependency)
#include <strong19/Strong.h>

template<class... Ts, class Char> struct fmt::formatter<partial19::PartialWhich<Ts...>, Char> {
    constexpr auto parse(fmt::basic_format_parse_context<Char>& ctx) { return ctx.begin(); }

    template<size_t I, class T> static auto format_type_index(bool hit, auto& first, auto& out) -> bool {
        if (hit) {
            if constexpr (strong19::is_strong<T>) {
                if (first) {
                    out = fmt::format_to(out, "{}", strong19::strong_name<T>);
                    first = false;
                }
                else {
                    out = fmt::format_to(out, ", {}", strong19::strong_name<T>);
                }
            }
            else {
                if (first) {
                    out = fmt::format_to(out, "{}", I);
                    first = false;
                }
                else {
                    out = fmt::format_to(out, ", {}", I);
                }
            }
        }
        return true;
    }

    template<typename FormatContext> auto format(partial19::PartialWhich<Ts...> const& v, FormatContext& ctx) const {
        if constexpr (sizeof...(Ts) == 0) {
            return fmt::format_to(ctx.out(), "Which()");
        }
        else {
            return [&]<size_t... Is>(std::index_sequence<Is...> const&) {
                auto out = fmt::format_to(ctx.out(), "Which<");
                bool first = true;
                ((format_type_index<Is, Ts>(v[Is], first, out)), ...);
                return fmt::format_to(out, ">");
            }(std::make_index_sequence<sizeof...(Ts)>{});
        }
    }
};
