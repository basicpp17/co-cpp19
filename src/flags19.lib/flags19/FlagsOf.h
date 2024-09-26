#pragma once
#include "enum19/Enum.h"
#include "enum19/Enum.max.h"

#include <stdint.h> // int64_t, uint64_t

namespace flags19 {

using enum19::HasMetaEnum;
using enum19::max_underlying_value_of;
using enum19::meta_enum_for;

namespace details {

template<size_t maxBit> constexpr auto storageTypeForMaxBit() {
    if constexpr (maxBit <= 8 * sizeof(uint8_t)) {
        return uint8_t{};
    }
    else if constexpr (maxBit <= 8 * sizeof(uint16_t)) {
        return uint16_t{};
    }
    else if constexpr (maxBit <= 8 * sizeof(uint32_t)) {
        return uint32_t{};
    }
    else if constexpr (maxBit <= 8 * sizeof(uint64_t)) {
        return uint64_t{};
    }
    else {
        static_assert(maxBit > 8 * sizeof(uint64_t), "not supported right now");
    }
}

} // namespace details

template<HasMetaEnum Enum> struct FlagsOf {
    using UnderlyingBit = std::underlying_type_t<Enum>;
    using Value = decltype(details::storageTypeForMaxBit<max_underlying_value_of<Enum>>());

    constexpr FlagsOf() = default;
    explicit constexpr FlagsOf(Value const& value) : m_value{value} {}

    template<class... Args> requires((sizeof...(Args) > 0) && ... && std::is_same_v<Args, Enum>)
    explicit constexpr FlagsOf(Args... args)
            : FlagsOf{static_cast<Value>(((1U << static_cast<UnderlyingBit>(args)) | ...))} {}

    auto operator==(FlagsOf const&) const -> bool = default;

    explicit operator Value() const { return m_value; }

    [[nodiscard]] constexpr auto operator[](Enum bit) const noexcept -> bool {
        return 0U != (m_value & (1U << static_cast<UnderlyingBit>(bit)));
    }

    template<class... Args> requires((sizeof...(Args) > 0) && ... && std::is_same_v<Args, Enum>)
    constexpr auto allOf(Args... args) const -> bool {
        auto const mask = ((1U << static_cast<UnderlyingBit>(args)) | ...);
        return mask == (m_value & mask);
    }

    template<class... Args> requires((sizeof...(Args) > 0) && ... && std::is_same_v<Args, Enum>)
    constexpr auto someOf(Args... args) const -> bool {
        auto const mask = ((1U << static_cast<UnderlyingBit>(args)) | ...);
        return 0U != (m_value & mask);
    }

    template<class... Args> requires((sizeof...(Args) > 0) && ... && std::is_same_v<Args, Enum>)
    constexpr auto noneOf(Args... args) const -> bool {
        auto const mask = ((1U << static_cast<UnderlyingBit>(args)) | ...);
        return 0U == (m_value & mask);
    }

    constexpr void resetAll() { m_value = {}; }

    template<class... Args> requires((sizeof...(Args) > 0) && ... && std::is_same_v<Args, Enum>)
    constexpr void set(Args... args) {
        m_value |= ((1U << static_cast<UnderlyingBit>(args)) | ...);
    }

    template<class... Args> requires((sizeof...(Args) > 0) && ... && std::is_same_v<Args, Enum>)
    constexpr void reset(Args... args) {
        m_value &= ~static_cast<Value>(((1U << static_cast<UnderlyingBit>(args)) | ...));
    }

    template<class... Args> requires((sizeof...(Args) > 0) && ... && std::is_same_v<Args, Enum>)
    constexpr void toggle(Args... args) {
        m_value ^= ((1U << static_cast<UnderlyingBit>(args)) | ...);
    }

    constexpr auto operator|(FlagsOf const& other) const -> FlagsOf {
        return FlagsOf{static_cast<Value>(m_value | other.m_value)};
    }
    constexpr auto operator&(FlagsOf const& other) const -> FlagsOf {
        return FlagsOf{static_cast<Value>(m_value & other.m_value)};
    }
    constexpr auto operator|=(FlagsOf const& other) -> FlagsOf& { return m_value |= other.m_value, *this; }
    constexpr auto operator&=(FlagsOf const& other) -> FlagsOf& { return m_value &= other.m_value, *this; }

private:
    Value m_value{};
};

} // namespace flags19
