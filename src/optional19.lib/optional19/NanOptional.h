#pragma once
#include "Optional.h"
#include "meta19/nullptr_to.h"

#include <limits> // std::numeric_limits
#include <type_traits> // std::is_invocable_r_v

namespace optional19 {

using meta19::nullptr_to;

template<class T> requires(std::numeric_limits<T>::has_quiet_NaN) struct NanPacked;

/// Specialized packed optional storing a floating point value
/// NaN (not a number) values are treated as not present values
template<class T> struct Optional<NanPacked<T>> {
private:
    static constexpr auto nan = std::numeric_limits<T>::quiet_NaN();
    static constexpr bool isNan(T v) { return v != v; } // note: Only NaN values are not equal to themself
    T m_data{nan};

public:
    constexpr Optional() = default;
    constexpr Optional(T v) : m_data(v) {}

    constexpr explicit operator bool() const { return !isNan(m_data); }

    /// @return predicate(value()) if optional is set else return false
    template<class F> constexpr auto operator&&(F&& f) const -> bool {
        if constexpr (std::is_invocable_r_v<bool, F, T>)
            return *this ? f(value()) : false;
        else {
            return *this ? (bool)f : false;
        }
    }

    template<class F> constexpr auto operator||(F&& f) const -> T {
        if constexpr (std::is_invocable_r_v<T, F>)
            return *this ? value() : f();
        else
            return *this ? value() : f;
    }

    template<class F> constexpr auto map(F&& f) const -> decltype(f(*nullptr_to<T>)) {
        if (*this) return f(value());
        if constexpr (!std::is_void_v<decltype(f(*nullptr_to<T>))>) return {};
    }

    constexpr auto value() const -> const T& { return m_data; }
    constexpr auto amend() -> T& { return m_data; }
};

} // namespace optional19
