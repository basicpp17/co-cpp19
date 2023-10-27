#pragma once
#include "Tuple.h"

/// note: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>

namespace tuple19 {

/// use values of the tuple as format arguments
/// note: you still have to craft the proper format string!
/// usage: vformat(formatStr, tuple19::make_format_args(argsTuple));
template<class... Ts> constexpr auto make_format_args(Tuple<Ts...> const& args) {
    return [&]<size_t... Is>(std::index_sequence<Is...> const&) {
        return fmt::make_format_args(args.template at<Is>()...);
    }
    (std::make_index_sequence<sizeof...(Ts)>{});
}

} // namespace tuple19

template<class... Ts, class Char> struct fmt::formatter<tuple19::Tuple<Ts...>, Char> {
    constexpr auto parse(fmt::basic_format_parse_context<Char>& ctx) { return ctx.begin(); }

    template<typename FormatContext> auto format(tuple19::Tuple<Ts...> const& v, FormatContext& ctx) {
        if constexpr (sizeof...(Ts) == 0) {
            return fmt::format_to(ctx.out(), "Tuple<>");
        }
        else {
            return [&]<size_t... Is>(std::index_sequence<0, Is...> const&) {
                auto out = fmt::format_to(ctx.out(), "{}", v.template at<0>());
                ((out = fmt::format_to(out, ", {}", v.template at<Is>())), ...);
                return out;
            }
            (std::make_index_sequence<sizeof...(Ts)>{});
        }
    }
};
