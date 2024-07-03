#pragma once
#include "FlagsOf.h"
#include "enum19/Enum.names.h"

/// noto: for this header you need fmt library (not included as library dependency)
#include <fmt/format.h>
#include <string_view>

/// adds enum support for fmt
template<enum19::HasMetaEnum Enum, class Char> struct fmt::formatter<flags19::FlagsOf<Enum>, Char> {
    using T = flags19::FlagsOf<Enum>;
    using Value = typename T::Value;
    constexpr auto parse(fmt::basic_format_parse_context<Char>& ctx) { return ctx.begin(); }

    template<typename FormatCtx> auto format(const T& v, FormatCtx& ctx) const {
        // auto underlying = static_cast<std::underlying_type_t<T>>(v);
        using namespace std::string_view_literals;
        auto printed = false;
        for (auto& member : enum19::meta_enum_for<Enum>.members) {
            if (!v[member.value]) continue;
            fmt::format_to(ctx.out(), "{}{}", (printed ? "|"sv : ""sv), enum19::valueName(member.value));
            printed = true;
        }
        return fmt::format_to(
            ctx.out(),
            "{} ({:0{}b})",
            (printed ? ""sv : "<>"sv),
            static_cast<Value>(v),
            enum19::max_underlying_value_of<Enum>);
    }
};
