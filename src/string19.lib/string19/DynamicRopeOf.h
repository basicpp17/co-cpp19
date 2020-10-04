#pragma once
#include "ADL.h"
#include "DynamicString.h"
#include "array19/DynamicArrayOf.h"
#include "variant19/Variant.h"

#include <type_traits>

namespace string19 {

using array19::DynamicArrayOf;
using variant19::Variant;

template<class A, class B> constexpr bool ropeCombine(ADL*, A&&, B&&) { return false; }

/// representation of a string of different parts
/// purpose:
/// * allows to efficiently build large strings
template<class... Ts> struct DynamicRopeOf {
    using Entry = Variant<Ts...>;
    using Container = DynamicArrayOf<Entry>;

private:
    template<class... Ps> friend struct DynamicRopeOf;
    Container m{};

public:
    DynamicRopeOf() = default;

    /// computes the total length of all Rope parts
    auto length() const -> size_t {
        auto result = size_t{};
        for (auto& entry : m) {
            entry.visit([&](const auto& v) { result += ropeLengthOf(adl, v); });
        }
        return result;
    }

    /// builds a dynamic string that contains all parts
    auto build() const -> DynamicString {
        auto result = DynamicString{};
        result.resize(length());
        auto it = result.data();
        for (auto& entry : m) {
            entry.visit([&it](const auto& v) { ropeAppend(adl, it, v); });
        }
        return result;
    }

    /// add parts of other Rope by moving all parts
    /// note: it might be more efficient to add rope as part
    template<class... Ps>
    requires((!std::is_same_v<DynamicRopeOf<Ps...>, Ts> && ...)) auto operator+=(DynamicRopeOf<Ps...>&& r)
        -> DynamicRopeOf& {
        m.ensureUnusedCapacity(r.m.count());
        for (auto& pEntry : r.m) {
            pEntry.visit([&](auto& v) { m.emplace_back(std::move(v)); });
        }
        return *this;
    }

    /// add one part to the Rope
    template<class P>
    requires((std::is_same_v<std::remove_cvref_t<P>, Ts> || ...)) auto operator+=(P&& v) -> DynamicRopeOf& {
        if (ropeLengthOf(adl, v) > 0) {
            if (m.isEmpty() || !ropeCombine(adl, m.back(), v)) {
                m.emplace_back((P &&) v);
            }
        }
        return *this;
    }
};

} // namespace string19
