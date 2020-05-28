#pragma once
#include "meta19/nullptr_to.h"

#include <type_traits> // std::is_invocable_r_v

namespace optional19 {

using meta19::nullptr_to;

/// PackedOptional saves the extra bool by using one value as invalid
/// note: we cannot pass any value to templates, so we pass a constexpr functor that returns the invalid value
template<auto makeInvalid> struct PackedOptional {
private:
    using T = decltype(makeInvalid());
    T m_data{makeInvalid()};

public:
    constexpr PackedOptional() = default;
    constexpr PackedOptional(T v) : m_data((T &&) v){};

    constexpr explicit operator bool() const { return !(m_data == makeInvalid()); }

    /// @return predicate(value()) if optional is set else return false
    template<class F> constexpr auto operator&&(F&& f) const -> bool {
        if constexpr (std::is_invocable_r_v<bool, F, T>)
            return *this ? f(value()) : false;
        else {
            return *this ? f : false;
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

/// helper function to use default value of T as invalid
/// usage: OptInt = PackedOptional<defaultOf<int>>;
template<class T> constexpr auto defaultOf() -> T { return {}; }

/// helper function to use nullptr as default for Pointer to T
/// usage: OptPtr = PackedOptional<ptrTo<void>>;
template<class T> constexpr auto nullptrTo() -> T* { return {}; }

} // namespace optional19
