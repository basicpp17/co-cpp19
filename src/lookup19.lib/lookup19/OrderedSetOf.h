#pragma once
#include "OrderedSliceOf.h"
#include "array19/AllocatedArrayUtils.h"

#include <stddef.h> // size_t

namespace lookup19 {

using array19::AllocatedArrayUtils;

struct DefaultLess {
    template<class A, class B> constexpr auto operator()(const A& a, const B& b) const { return a < b; }
};

/// Useful for trivial sortable types
template<class T, class Less = DefaultLess> struct OrderedSetOf {
    using Element = T;
    using Count = size_t;
    using Index = size_t;

    using Iterator = Element*;
    using ConstIterator = const Element*;
    using Slice = OrderedSliceOf<const T, Less>;
    using AmendableSlice = SliceOf<T>;
    using UnorderedSlice = SliceOf<const T>;
    using MoveSlice = array19::MoveSliceOf<T>;

    static_assert(std::is_trivial_v<T>, "Only works for trivial types!");

private:
    using Utils = AllocatedArrayUtils<T>;

    T* m_pointer{};
    Count m_count{};
    Count m_capacity{};

public:
    constexpr OrderedSetOf() = default;
    explicit constexpr OrderedSetOf(Slice ordered) noexcept
            : m_pointer{Utils::allocate(ordered.count())}
            , m_count{ordered.count()}
            , m_capacity{ordered.count()} {
        Utils::copyAssign(m_pointer, ordered);
    }
    constexpr OrderedSetOf(const OrderedSetOf& o) noexcept
            : m_pointer{Utils::allocate(o.m_count)}
            , m_count{o.m_count}
            , m_capacity{o.m_count} {
        Utils::copyAssign(m_pointer, Slice{o});
    }
    constexpr OrderedSetOf(OrderedSetOf&& o) noexcept
            : m_pointer{std::exchange(o.m_pointer, nullptr)}
            , m_count{o.m_count}
            , m_capacity{o.m_count} {}
    constexpr auto operator=(const OrderedSetOf& o) noexcept -> OrderedSetOf& {
        if (o.m_count > m_capacity) {
            if (m_pointer) Utils::deallocate(SliceOf{m_pointer, m_capacity});
            m_pointer = Utils::allocate(o.m_count);
            m_capacity = o.m_count;
        }
        Utils::copyAssign(m_pointer, Slice{o});
        m_count = o.m_count;
        return *this;
    }
    constexpr auto operator=(OrderedSetOf&& o) noexcept -> OrderedSetOf& {
        if (m_pointer) Utils::deallocate(SliceOf{m_pointer, m_capacity});
        m_pointer = std::exchange(o.m_pointer, nullptr);
        m_count = o.m_count;
        m_capacity = o.m_capacity;
        return *this;
    }
    constexpr ~OrderedSetOf() noexcept {
        if (m_pointer) Utils::deallocate(SliceOf{m_pointer, m_capacity});
    }

    [[nodiscard]] constexpr auto isEmpty() const -> bool { return m_count == 0; }
    [[nodiscard]] constexpr auto count() const -> Count { return m_count; }
    [[nodiscard]] constexpr auto totalCapacity() const -> Count { return m_capacity; }
    [[nodiscard]] constexpr auto unusedCapacity() const -> Count { return m_capacity - m_count; }

    [[nodiscard]] constexpr auto front() const -> const T& { return *begin(); }
    [[nodiscard]] constexpr auto back() const -> const T& { return *(begin() + m_count - 1); }

    [[nodiscard]] constexpr auto begin() const noexcept -> ConstIterator { return m_pointer; }
    [[nodiscard]] constexpr auto end() const noexcept -> ConstIterator { return begin() + m_count; }
    [[nodiscard]] constexpr auto operator[](Index index) const -> const Element& { return m_pointer[index]; }
    [[nodiscard]] constexpr operator Slice() const { return Slice{m_pointer, m_count}; }

    constexpr void ensureCapacity(Count count) {
        if (totalCapacity() < count) growBy(static_cast<size_t>(count - totalCapacity()));
    }
    constexpr void ensureUnusedCapacity(Count count) {
        if (unusedCapacity() < count) growBy(static_cast<size_t>(count - unusedCapacity()));
    }

    /// inserts a single value to the set if it is not yet present
    /// note:
    /// * if you want to insert muliple values use merge
    constexpr auto insert(T v) -> bool {
        auto it = const_cast<Iterator>(static_cast<Slice>(*this).lowerBound(v));
        if (it != end() && *it == v) {
            return false;
        }
        if (m_count == m_capacity) {
            auto newStorage = grownStorage(1);
            auto nPtr = newStorage.begin();
            auto fCount = static_cast<size_t>(it - m_pointer);
            if (0 != fCount) {
                Utils::moveConstruct(nPtr, MoveSlice{m_pointer, fCount});
                nPtr += fCount;
            }
            *nPtr++ = v;
            if (fCount != m_count) {
                Utils::moveConstruct(nPtr, MoveSlice{m_pointer + fCount, m_count - fCount});
            }
            Utils::deallocate(SliceOf{m_pointer, m_capacity});
            m_pointer = newStorage.begin();
            m_capacity = newStorage.count();
            m_count++;
            return true;
        }
        if (it != end()) {
            Utils::moveAssignReverse(it + 1, MoveSlice{it, static_cast<size_t>(end() - it)});
        }
        *it = v;
        m_count++;
        return true;
    }

    /// removes a single element from the set
    /// preconditions:
    /// * cIt has to point between begin() and before end()
    constexpr void remove(ConstIterator cIt) {
        auto it = const_cast<Iterator>(cIt);
        if (it + 1 != end()) {
            Utils::moveAssignForward(it, MoveSlice{it + 1, static_cast<size_t>(end() - it)});
        }
        m_count--;
    }

    /// adds multiple elements to the set if they are not already present
    /// preconditions:
    /// * assumes that elems are unique and ordered
    constexpr void merge(Slice elems) {
        if (elems.isEmpty()) return;
        if (isEmpty()) {
            *this = OrderedSetOf{elems};
            return;
        }
        auto nCount = elems.count();
        if (unusedCapacity() < nCount) {
            auto newStorage = grownStorage(nCount);
            auto ptr = mergeInto(newStorage, elems);
            Utils::deallocate(SliceOf{m_pointer, m_capacity});
            m_pointer = newStorage.begin();
            m_capacity = newStorage.count();
            m_count = static_cast<size_t>(ptr - m_pointer);
        }
        else {
            m_count = mergeBackwards(elems);
        }
    }

private:
    [[nodiscard]] constexpr auto grownStorage(size_t growBy) const -> AmendableSlice {
        auto cur = m_capacity;
        auto res = (cur << 1) - (cur >> 1) + (cur >> 4); // * 1.563
        if (res < 5) res = 5;
        if (res < m_capacity + growBy) res = m_capacity + growBy;
        auto ptr = Utils::allocate(res);
        return AmendableSlice{ptr, res};
    }
    constexpr void growBy(size_t by) {
        auto newStorage = grownStorage(by);
        Utils::moveConstruct(newStorage.begin(), MoveSlice{m_pointer, m_count});
        Utils::deallocate(SliceOf{m_pointer, m_capacity});
        m_pointer = newStorage.begin();
        m_capacity = newStorage.count();
    }
    constexpr auto mergeInto(AmendableSlice storage, Slice elems) -> T* {
        auto less = Less{};
        auto dPtr = storage.begin();
        auto nPtr = elems.begin();
        auto const nEnd = elems.end();
        auto n = *nPtr;
        auto oPtr = begin();
        auto o = *oPtr;
        auto const oEnd = end();
        while (true) {
            if (less(n, o)) {
                *dPtr++ = n;
                nPtr++;
                if (nPtr == nEnd) {
                    auto r = static_cast<size_t>(oEnd - oPtr);
                    Utils::copyAssign(dPtr, UnorderedSlice{oPtr, r});
                    return dPtr + r;
                }
                n = *nPtr;
            }
            else {
                if (less(o, n)) {
                    *dPtr++ = o;
                }
                oPtr++;
                if (oPtr == oEnd) {
                    auto r = static_cast<size_t>(nEnd - nPtr);
                    Utils::copyAssign(dPtr, UnorderedSlice{nPtr, r});
                    return dPtr + r;
                }
                o = *oPtr;
            }
        }
    }
    constexpr auto mergeBackwards(Slice elems) -> size_t {
        auto less = Less{};
        auto const oBegin = m_pointer;
        auto const oCount = m_count;
        auto const nBegin = elems.begin();
        auto const nCount = elems.count();
        auto oIt = oBegin + oCount - 1;
        auto nIt = nBegin + nCount - 1;
        auto dCount = oCount + nCount;
        auto dIt = oBegin + dCount - 1;

        auto o = *oIt;
        auto n = *nIt;
        while (true) {
            if (less(o, n)) {
                *dIt-- = n;
                if (nIt == nBegin) {
                    dIt++;
                    oIt++;
                    if (oIt != dIt) { // move by skipped elements
                        dCount -= (dIt - oIt);
                        Utils::moveAssignForward(oIt, MoveSlice{dIt, static_cast<size_t>(dCount - (oIt - oBegin))});
                    }
                    // remaining old values are in place
                    return dCount;
                }
                nIt--;
                n = *nIt;
            }
            else {
                if (less(n, o)) {
                    *dIt-- = o;
                }
                if (oIt == oBegin) {
                    auto const remaining = static_cast<size_t>(nIt + 1 - nBegin);
                    Utils::copyAssign(oBegin, UnorderedSlice{nBegin, remaining});
                    dIt++;
                    if (oBegin + remaining != dIt) { // move by skipped elements
                        dCount -= (dIt - (oBegin + remaining));
                        Utils::moveAssignForward(
                            oBegin + remaining,
                            MoveSlice{dIt, static_cast<size_t>(dCount - remaining)});
                    }
                    return dCount;
                }
                oIt--;
                o = *oIt;
            }
        }
    }
};

/// deduce OrderedSliceOf from OrderedSetOf
/// usage:
///     auto a = OrderedSetOf{1,2,3};
///     auto slice = OrderedSliceOf{a};
template<class T, class L> OrderedSliceOf(const OrderedSetOf<T, L>&) -> OrderedSliceOf<const T, L>;

} // namespace lookup19
