#pragma once
#include "meta19/nullptr_to.h"

#include <cstdint> // uint8_t
#include <new> // std::launder
#include <type_traits> // std::is_trivially_destructable_v
#include <utility>

namespace optional19 {

using meta19::nullptr_to;

struct Inplace;
constexpr static auto inplace = nullptr_to<Inplace>;

namespace details {

template<class T> struct OptionalStorage {
    using Storage = uint8_t[sizeof(T)];
    alignas(T) Storage m_storage{};
    bool m_valid{};

    OptionalStorage() = default;
    ~OptionalStorage() noexcept {
        if (m_valid) {
            auto p = std::launder(reinterpret_cast<T*>(m_storage));
            p->~T();
        }
    }
    OptionalStorage(T v) {
        new (m_storage) T(std::move(v));
        m_valid = true;
    }
    template<class... Args> OptionalStorage(Inplace*, Args&&... args) {
        new (m_storage) T{(Args &&) args...};
        m_valid = true;
    }
    OptionalStorage(const OptionalStorage& o) noexcept(std::is_nothrow_copy_constructible_v<T>) {
        if (o.m_valid) {
            new (m_storage) T(o.value());
            m_valid = true;
        }
    }
    OptionalStorage& operator=(const OptionalStorage& o) noexcept(std::is_nothrow_copy_assignable_v<T>) {
        if (o.m_valid) {
            if (m_valid)
                amend() = o.value();
            else {
                new (m_storage) T(o.value());
                m_valid = true;
            }
        }
        else if (m_valid) {
            amend().~T();
            m_valid = false;
        }
        return *this;
    }
    OptionalStorage(OptionalStorage&& o) noexcept {
        if (o.m_valid) {
            new (m_storage) T(std::move(o.amend()));
            m_valid = true;
        }
    }
    OptionalStorage& operator=(OptionalStorage&& o) noexcept {
        if (o.m_valid) {
            if (m_valid)
                amend() = std::move(o.amend());
            else {
                new (m_storage) T(std::move(o.amend()));
                m_valid = true;
            }
        }
        else if (m_valid) {
            amend().~T();
            m_valid = false;
        }
        return *this;
    }

    template<class... Args> constexpr void emplace(Args&&... args) {
        if (this->m_valid) amend().~T();
        new (this->m_storage) T{(Args &&) args...};
        this->m_valid = true;
    }

    auto value() const -> const T& { return *std::launder(reinterpret_cast<const T*>(this->m_storage)); }
    auto amend() -> T& { return *std::launder(reinterpret_cast<T*>(this->m_storage)); }
};

/// simplified specialization if no destructor is required
/// * storing actual type saves a lot of casting overhead
template<class T> requires(std::is_trivially_default_constructible_v<T> && std::is_trivially_destructible_v<T>)
struct OptionalStorage<T> {
    T m_value{};
    bool m_valid{};

    constexpr OptionalStorage() = default;
    constexpr OptionalStorage(T v) : m_value(std::move(v)), m_valid(true) {}
    template<class... Args> OptionalStorage(Inplace*, Args&&... args) : m_value{(Args &&) args...}, m_valid(true) {}

    template<class... Args> constexpr void emplace(Args&&... args) {
        m_value = T{(Args &&) args...};
        m_valid = true;
    }

    constexpr auto value() const -> const T& { return m_value; }
    constexpr auto amend() -> T& { return m_value; }
};

} // namespace details

template<class T> struct Optional : private details::OptionalStorage<T> {
private:
    using Base = details::OptionalStorage<T>;

public:
    using Value = T;

    using Base::Base;
    using Base::emplace;

    using Base::amend;
    using Base::value;

    constexpr explicit operator bool() const { return Base::m_valid; }

    /// @return predicate(value()) if optional is set else return false
    template<class F> requires(std::is_invocable_r_v<bool, F, T>) constexpr auto operator&&(F&& f) const -> bool {
        return static_cast<bool>(*this) && f(value());
    }

    template<class F> constexpr auto operator||(F&& f) const -> T {
        if constexpr (std::is_invocable_r_v<T, F>)
            return Base::m_valid ? value() : f();
        else
            return Base::m_valid ? value() : f;
    }

    template<class F> constexpr auto map(F&& f) const -> decltype(f(*nullptr_to<T>)) {
        if (Base::m_valid) return f(value());
        if constexpr (!std::is_void_v<decltype(f(*nullptr_to<T>))>) return {};
    }
};

} // namespace optional19
