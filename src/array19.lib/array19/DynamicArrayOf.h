#pragma once
#include "AllocatedArrayUtils.h"
#include "SliceOf.single.h"

#include <new> // launder

namespace array19 {

/// replacement for std::vector with different API
/// * focus on SliceOf
/// * mutable is explicit - use amend()
template<class T> struct DynamicArrayOf final {
    using Element = T;
    using Count = size_t;
    using Index = size_t;

    using Iterator = Element*;
    using ConstIterator = const Element*;
    using Slice = SliceOf<Element>;
    using ConstSlice = SliceOf<const Element>;

private:
    using Utils = AllocatedArrayUtils<T>;

    T* m_pointer;
    Count m_count;
    Count m_capacity;

public:
    DynamicArrayOf() = default;
    ~DynamicArrayOf() noexcept {
        if (m_pointer) {
            Utils::destruct(amend());
            Utils::deallocate(Slice{m_pointer, m_capacity});
        }
    }

    DynamicArrayOf(ConstSlice slice) { append(slice); }

    template<class... Ts> requires(sizeof...(Ts) > 0) && requires(Ts... args) { (T(args), ...); }
    DynamicArrayOf(Ts&&... args) noexcept(std::is_nothrow_copy_constructible_v<Element>)
            : m_pointer(Utils::allocate(sizeof...(Ts)))
            , m_count(0)
            , m_capacity(sizeof...(Ts)) {
        (Utils::copyConstruct(m_pointer + m_count++, sliceOfSingle<const T>(args)), ...);
    }

    DynamicArrayOf(const DynamicArrayOf& o) noexcept(std::is_nothrow_copy_constructible_v<Element>)
            : m_pointer(Utils::allocate(o.m_count))
            , m_count(o.m_count) {
        Utils::copyConstruct(m_pointer, o);
    }
    DynamicArrayOf& operator=(const DynamicArrayOf& o) noexcept(std::is_nothrow_copy_assignable_v<Element>) {
        if (m_capacity < o.m_count) {
            *this = DynamicArrayOf(o);
        }
        else {
            auto countDiff = m_count - o.m_count;
            if (countDiff > 0) {
                Utils::copyAssign(amendBegin(), o);
                Utils::destruct(Slice{amendBegin() + o.m_count, countDiff});
            }
            else {
                Utils::copyAssign(amendBegin(), ConstSlice{o.begin(), m_count});
                Utils::copyConstruct(storageEnd(), ConstSlice{o.begin() + m_count, -countDiff});
            }
            m_count = o.m_count;
        }
        return *this;
    }

    DynamicArrayOf(DynamicArrayOf&& o) noexcept : m_pointer(o.m_pointer), m_count(o.m_count), m_capacity(o.m_capacity) {
        o.m_pointer = nullptr;
    }
    DynamicArrayOf& operator=(DynamicArrayOf&& o) noexcept {
        Utils::destruct(amend());
        Utils::deallocate(Slice{m_pointer, m_capacity});
        m_pointer = o.m_pointer;
        m_count = o.m_count;
        m_capacity = o.m_capacity;
        o.m_pointer = nullptr;
        return *this;
    }

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] auto count() const -> Count { return m_count; }
    [[nodiscard]] auto totalCapacity() const -> Count { return m_capacity; }
    [[nodiscard]] auto unusedCapacity() const -> Count { return m_capacity - m_count; }

    [[nodiscard]] auto front() const -> const T& { return *begin(); }
    [[nodiscard]] auto back() const -> const T& { return *(begin() + m_count - 1); }

    [[nodiscard]] auto begin() const noexcept -> ConstIterator {
        return std::launder(reinterpret_cast<const T*>(m_pointer));
    }
    [[nodiscard]] auto end() const noexcept -> ConstIterator { return begin() + m_count; }
    [[nodiscard]] auto operator[](Index index) const noexcept -> const Element& {
        return *std::launder(reinterpret_cast<const T*>(m_pointer + index));
    }
    [[nodiscard]] operator ConstSlice() const noexcept { return ConstSlice{begin(), m_count}; }

    [[nodiscard]] auto amendBegin() & noexcept -> Iterator { return std::launder(reinterpret_cast<T*>(m_pointer)); }
    [[nodiscard]] auto amendEnd() & noexcept -> Iterator { return amendBegin() + m_count; }

    [[nodiscard]] auto amend() -> Slice { return Slice{amendBegin(), m_count}; }

    void ensureCapacity(Count count) {
        if (totalCapacity() < count) growBy(count - totalCapacity());
    }
    void ensureUnusedCapacity(Count count) {
        if (unusedCapacity() < count) growBy(count);
    }

    /// append a single element constructed in place with the given arguments
    template<class... Ts> void emplace_back(Ts&&... args) {
        ensureUnusedCapacity(1);
        new (m_pointer + m_count) Element((Ts &&) args...);
        m_count++;
    }

    /// appends possibly multiple elements at the end
    void append(ConstSlice elems) {
        ensureUnusedCapacity(elems.count());
        Utils::copyConstruct(storageEnd(), elems);
        m_count += elems.count();
    }

    /// removes the last element
    void pop() noexcept(std::is_nothrow_destructible_v<Element>) {
        Utils::destruct(Slice{amendBegin() + m_count - 1, 1});
        m_count--;
    }

    /// swiss army knife of mutating an dynamic array
    /// replaces:
    /// * insert - splice(it, 0, sliceOfSingle{V})
    /// * erase - splice(it, N, {})
    /// extensions:
    /// * insert of elements at once
    /// * combined remove and insert
    /// known limits:
    /// * no in place or move construction (not needed so far)
    void splice(Iterator it, Count removeCount, ConstSlice insertSlice) {
        auto offset = it - amendBegin();
        auto insertCount = insertSlice.count();
        auto remainCount = m_count - offset - removeCount;
        auto remainSlice = Slice{it + removeCount, remainCount};
        // old: [ ..(offset)..,[it] ..(removeCount).., ..(remainCount)... ]
        // new: [ ..(offset)..,[it] ..(insertCount).., ..(remainCount)... ]

        auto countDiff = insertCount - removeCount;
        if (unusedCapacity() < countDiff) { // not enough storage => arrange everything in new storage
            auto newStorage = grownStorage(countDiff);
            Utils::moveConstruct(newStorage.begin(), Slice{amendBegin(), static_cast<size_t>(offset)});
            Utils::copyConstruct(newStorage.begin() + offset, insertSlice);
            Utils::moveConstruct(newStorage.begin() + offset + insertCount, remainSlice);
            Utils::destruct(amend());
            Utils::deallocate(Slice{m_pointer, m_capacity});
            m_pointer = newStorage.begin();
            m_capacity = newStorage.count();
        }
        else if (countDiff <= 0) { // shrinking
            Utils::copyAssign(it, insertSlice);
            Utils::moveConstructForward(m_pointer + offset + insertCount, remainSlice);
            Utils::destruct(Slice{amendEnd() + countDiff, -countDiff});
        }
        else if (offset + insertCount <= m_count) { // parts of remainSlice is moved beyond end()
            Utils::moveConstruct(storageEnd(), remainSlice.slice(remainCount - countDiff, countDiff));
            Utils::moveAssignReverse(it + insertCount, remainSlice.slice(0, remainCount - countDiff));
            Utils::copyAssign(it, insertSlice);
        }
        else { // remainSlice is moved beyond end()
            Utils::moveConstruct(m_pointer + offset + insertCount, remainSlice);
            auto assignElems = m_count - offset;
            Utils::copyAssign(it, insertSlice.slice(0, assignElems));
            Utils::copyConstruct(storageEnd(), insertSlice.slice(assignElems, insertCount - assignElems));
        }
        m_count += countDiff;
    }

private:
    [[nodiscard]] auto byteSize() const -> size_t { return m_count * sizeof(T); }
    [[nodiscard]] auto storageEnd() -> Iterator { return m_pointer + m_count; }

    [[nodiscard]] auto grownStorage(int growBy) const -> Slice {
        auto cur = m_capacity;
        auto res = (cur << 1) - (cur >> 1) + (cur >> 4); // * 1.563
        if (res < 5) res = 5;
        if (res < m_capacity + growBy) res = m_capacity + growBy;
        auto ptr = Utils::allocate(res);
        return Slice{ptr, res};
    }
    void growBy(int by) {
        auto newStorage = grownStorage(by);
        Utils::moveConstruct(newStorage.begin(), amend());
        Utils::destruct(amend());
        Utils::deallocate(Slice{m_pointer, m_capacity});
        m_pointer = newStorage.begin();
        m_capacity = newStorage.count();
    }
};

/// simplified deduction guide
/// usage:
///     DynamicArrayOf{1,2,3}
template<class T, class... Ts> DynamicArrayOf(T&&, Ts&&...) -> DynamicArrayOf<T>;

/// deduce SliceOf from DynamicArray
/// usage:
///     auto a = DynamicArrayOf{1,2,3;
///     auto slice = SliceOf{a};
template<class T> SliceOf(const DynamicArrayOf<T>&) -> SliceOf<const T>;

} // namespace array19
