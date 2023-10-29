#pragma once
#include "AllocatedArrayUtils.h"
#include "Span.one.h"

#include <new> // new allocators
#include <stddef.h> // size_t
#include <type_traits> // is_nothrow_*

namespace array19 {

/// allocated array on the heap
/// * count is fixated at construction time
/// * all array elements are initialized (unlike std::vector)
///
/// note:
/// * C++ lacks a new T[] (custom-initializer) thing - so we would not be able to copy without default initializing
///   => to work around this we use ::operator new[] & ::operator delete[] and construct explicitly
template<class T> struct AllocatedArray final {
    using Data = T;
    using Count = size_t;
    using Index = size_t;

    using ConstIterator = Data const*;
    using Iterator = Data*;
    using ConstSpan = Span<Data const>;
    using AmendSpan = Span<Data>;
    using MoveSpan = Span<Data&&>;

private:
    using Utils = AllocatedArrayUtils<Data>;
    Data* m_data{};
    Count m_count{}; // equals capacity

public:
    AllocatedArray() = default;
    ~AllocatedArray() noexcept {
        if (m_data) {
            Utils::destruct(amend());
            Utils::deallocate(amend());
        }
    }

    explicit AllocatedArray(ConstSpan span) : m_data{Utils::allocate(span.count())}, m_count{span.count()} {
        Utils::copyConstruct(m_data, span);
    }

    explicit AllocatedArray(MoveSpan span) : m_data{Utils::allocate(span.count())}, m_count{span.count()} {
        Utils::moveConstruct(m_data, span);
    }

    template<will_construct<T>... Ts> requires(sizeof...(Ts) > 0)
    explicit AllocatedArray(Ts&&... args) : m_data{Utils::allocate(sizeof...(Ts))} {
        (new (m_data + m_count++) Data{(Ts &&) args}, ...);
    }

    AllocatedArray(AllocatedArray const& o) : AllocatedArray(Span{o}) {}
    AllocatedArray& operator=(AllocatedArray const& o) {
        if (!m_data || o.m_count != m_count) {
            *this = AllocatedArray(o);
        }
        else if (0 < m_count) {
            Utils::copyAssign(m_data, o);
        }
        return *this;
    }

    AllocatedArray(AllocatedArray&& o) noexcept : m_data{std::exchange(o.m_data, nullptr)}, m_count{o.m_count} {}
    AllocatedArray& operator=(AllocatedArray&& o) noexcept {
        if (m_data) {
            Utils::destruct(amend());
            Utils::deallocate(amend());
        }
        m_data = std::exchange(o.m_data, nullptr);
        m_count = o.m_count;
        return *this;
    }

    [[nodiscard]] static auto createCount(Count count) -> AllocatedArray {
        auto result = AllocatedArray{};
        result.m_data = Utils::allocate(count);
        Utils::defaultConstruct(AmendSpan{result.m_data, count});
        result.m_count = count;
        return result;
    }

    [[nodiscard]] auto isEmpty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] auto count() const noexcept -> Count { return m_count; }

    [[nodiscard]] auto begin() const noexcept -> ConstIterator { return std::launder(m_data); }
    [[nodiscard]] auto end() const noexcept -> ConstIterator { return begin() + m_count; }

    // requires: m_data && m_count != 0
    [[nodiscard]] auto front() const -> const Data& { return *begin(); }
    // requires: m_data && m_count != 0
    [[nodiscard]] auto back() const -> const Data& { return *(begin() + m_count - 1); }

    // requires: index < m_count
    [[nodiscard]] auto operator[](Index index) const noexcept -> Data const& { return *std::launder(m_data + index); }

    [[nodiscard]] operator ConstSpan() const noexcept { return ConstSpan{begin(), m_count}; }
    [[nodiscard]] auto amend() noexcept -> AmendSpan { return AmendSpan{std::launder(m_data), m_count}; }
    [[nodiscard]] auto move() noexcept -> MoveSpan { return MoveSpan{std::launder(m_data), m_count}; }
};

/// simplified deduction guide
/// usage:
///     AllocatedArray{1,2,3}
template<class T, class... Ts> AllocatedArray(T&&, Ts&&...) -> AllocatedArray<T>;

/// deduce Span from AllocatedArray
/// usage:
///     auto a = AllocatedArray{1,2,3};
///     auto span = Span{a};
template<class T> Span(AllocatedArray<T> const&) -> Span<T const>;

} // namespace array19
