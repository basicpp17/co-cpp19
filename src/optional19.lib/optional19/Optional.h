#pragma once
#include "meta19/nullptr_to.h"

#include <new> // std::launder
#include <stdint.h> // uint8_t
#include <type_traits> // std::is_trivially_destructable_v

namespace optional19 {

using meta19::nullptr_to;

namespace details {

template<class T, bool noDestructor> struct OptionalStorage {};

template<class T> struct OptionalStorage<T, true> {
    using Storage = uint8_t[sizeof(T)];
    T m_value{};
    bool m_valid{};

    constexpr OptionalStorage() = default;
    constexpr OptionalStorage(T v) : m_value(std::move(v)), m_valid(true) {}

    template<class... Args> static constexpr auto fromArgs(Args&&... args) -> OptionalStorage {
        return OptionalStorage{T{(Args &&) args...}, true};
    }
    template<class... Args> constexpr void emplace(Args&&... args) {
        m_value = T((Args &&) args...);
        m_valid = true;
    }

    constexpr auto value() const -> const T& { return m_value; }
    constexpr auto amend() -> T& { return m_value; }
};

template<class T> struct OptionalStorage<T, false> {
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
        new (this->m_storage) T(std::move(v));
        this->m_valid = true;
    }
    OptionalStorage(const OptionalStorage& o) noexcept(std::is_nothrow_copy_constructible_v<T>) {
        if (o.m_valid) {
            new (this->m_storage) T(o.value());
            this->m_valid = true;
        }
    }
    OptionalStorage& operator=(const OptionalStorage& o) noexcept(std::is_nothrow_copy_assignable_v<T>) {
        if (o.m_valid) {
            if (this->m_valid)
                amend() = o.value();
            else {
                new (this->m_storage) T(o.value());
                this->m_valid = true;
            }
        }
        else if (this->m_valid) {
            amend().~T();
            this->m_valid = false;
        }
        return *this;
    }
    OptionalStorage(OptionalStorage&& o) noexcept {
        if (o.m_valid) {
            new (this->m_storage) T(std::move(o.amend()));
            this->m_valid = true;
        }
    }
    OptionalStorage& operator=(OptionalStorage&& o) noexcept {
        if (o.m_valid) {
            if (this->m_valid)
                amend() = std::move(o.amend());
            else {
                new (this->m_storage) T(std::move(o.amend()));
                this->m_valid = true;
            }
        }
        else if (this->m_valid) {
            amend().~T();
            this->m_valid = false;
        }
        return *this;
    }

    template<class... Args> static constexpr auto fromArgs(Args&&... args) {
        auto result = OptionalStorage();
        new (result.m_storage) T((Args &&) args...);
        result.m_valid = true;
        return result;
    }
    template<class... Args> constexpr void emplace(Args&&... args) {
        if (this->m_valid) amend().~T();
        new (this->m_storage) T((Args &&) args...);
        this->m_valid = true;
    }

    auto value() const -> const T& { return *std::launder(reinterpret_cast<const T*>(this->m_storage)); }
    auto amend() -> T& { return *std::launder(reinterpret_cast<T*>(this->m_storage)); }
};

} // namespace details

template<class T>
struct Optional
        : private details::
              OptionalStorage<T, std::is_trivially_default_constructible_v<T> && std::is_trivially_destructible_v<T>> {
    using Base = details::
        OptionalStorage<T, std::is_trivially_default_constructible_v<T> && std::is_trivially_destructible_v<T>>;

    using Base::Base;
    using Base::emplace;
    using Base::fromArgs;

    using Base::amend;
    using Base::value;

    constexpr explicit operator bool() const { return this->m_valid; }

    /// @return predicate(value()) if optional is set else return false
    template<class F> constexpr auto operator&&(F&& f) const -> bool {
        if constexpr (std::is_invocable_r_v<bool, F, T>)
            return this->m_valid && f(value());
        else {
            return this->m_valid && f;
        }
    }

    template<class F> constexpr auto operator||(F&& f) const -> T {
        if constexpr (std::is_invocable_r_v<T, F>)
            return this->m_valid ? value() : f();
        else
            return this->m_valid ? value() : f;
    }

    template<class F> constexpr auto map(F&& f) const -> decltype(f(*nullptr_to<T>)) {
        if (this->m_valid) return f(value());
        if constexpr (!std::is_void_v<decltype(f(*nullptr_to<T>))>) return {};
    }
};

} // namespace optional19
