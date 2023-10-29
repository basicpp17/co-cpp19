#pragma once
#include "AllocatedArrayUtils.h"
#include "Span.one.h"

#include <new> // launder
#include <stddef.h> // size_t

namespace array19 {

/// replacement for std::vector with different API
/// * focus on Span
/// * mutable is explicit - use amend()
template<class T> struct DynamicArray final {
    using Data = T;
    using Count = size_t;
    using Index = size_t;

    using ConstIterator = Data const*;
    using AmendIterator = Data*;
    using ConstSpan = Span<Data const>;
    using AmendSpan = Span<Data>;
    using MoveSpan = Span<Data&&>;

private:
    using Utils = AllocatedArrayUtils<Data>;

    Data* m_data{};
    Count m_count{};
    Count m_capacity{};

public:
    DynamicArray() = default;
    ~DynamicArray() noexcept {
        if (m_data) {
            Utils::destruct(amend());
            Utils::deallocate(_amendCapacity());
        }
    }

    explicit DynamicArray(ConstSpan span)
            : m_data{Utils::allocate(span.count())}
            , m_count{span.count()}
            , m_capacity{span.count()} {
        Utils::copyConstruct(m_data, span);
    }

    explicit DynamicArray(MoveSpan span)
            : m_data{Utils::allocate(span.count())}
            , m_count{span.count()}
            , m_capacity{span.count()} {
        Utils::moveConstruct(m_data, span);
    }

    template<will_construct<Data>... Ts> requires(sizeof...(Ts) > 0)
    explicit DynamicArray(Ts&&... args) : m_data{Utils::allocate(sizeof...(Ts))}
                                        , m_capacity{sizeof...(Ts)} {
        (new (m_data + m_count++) Data{(Ts &&) args}, ...);
    }

    DynamicArray(DynamicArray const& o)
            : m_data{Utils::allocate(o.m_count)}
            , m_count{o.m_count}
            , m_capacity{o.m_count} {
        Utils::copyConstruct(m_data, o);
    }
    DynamicArray& operator=(DynamicArray const& o) {
        if (!m_data || m_capacity < o.m_count) {
            *this = DynamicArray(o);
        }
        else {
            if (m_count > o.m_count) {
                Utils::copyAssign(_amendBegin(), o);
                Utils::destruct(amend().skip(o.m_count));
            }
            else {
                Utils::copyAssign(_amendBegin(), ConstSpan{o}.head(m_count));
                Utils::copyConstruct(_vacantBegin(), ConstSpan{o}.skip(m_count));
            }
            m_count = o.m_count;
        }
        return *this;
    }

    DynamicArray(DynamicArray&& o) noexcept
            : m_data(std::exchange(o.m_data, nullptr))
            , m_count(o.m_count)
            , m_capacity(o.m_capacity) {}
    DynamicArray& operator=(DynamicArray&& o) noexcept {
        if (m_data) {
            Utils::destruct(amend());
            Utils::deallocate(_amendCapacity());
        }
        m_data = std::exchange(o.m_data, nullptr);
        m_count = o.m_count;
        m_capacity = o.m_capacity;
        return *this;
    }

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] auto count() const -> Count { return m_count; }
    [[nodiscard]] auto totalCapacity() const -> Count { return m_capacity; }
    [[nodiscard]] auto unusedCapacity() const -> Count { return m_capacity - m_count; }

    // requires: m_count > 0
    [[nodiscard]] auto front() const -> Data const& { return *begin(); }
    // requires: m_count > 0
    [[nodiscard]] auto back() const -> Data const& { return *(begin() + m_count - 1); }

    [[nodiscard]] auto begin() const noexcept -> ConstIterator { return std::launder(m_data); }
    [[nodiscard]] auto end() const noexcept -> ConstIterator { return begin() + m_count; }

    // requires: index < m_count
    [[nodiscard]] auto operator[](Index index) const noexcept -> Data const& { return *(begin() + index); }

    [[nodiscard]] operator ConstSpan() const noexcept { return ConstSpan{begin(), m_count}; }
    [[nodiscard]] auto amend() -> AmendSpan { return AmendSpan{_amendBegin(), m_count}; }
    [[nodiscard]] auto move() -> MoveSpan { return MoveSpan{_amendBegin(), m_count}; }

    void ensureCapacity(Count count) {
        if (totalCapacity() < count) _growBy(count - totalCapacity());
    }
    void ensureUnusedCapacity(Count count) {
        if (unusedCapacity() < count) _growBy(count - unusedCapacity());
    }

    void clear() {
        Utils::destruct(amend());
        m_count = 0;
    }

    /// append a single element constructed in place with the given arguments
    template<class... Ts> auto emplace_back(Ts&&... args) -> Data& {
        ensureUnusedCapacity(1);
        auto ptr = new (_vacantBegin()) Data{(Ts &&) args...};
        m_count++;
        return *ptr;
    }

    /// appends possibly multiple elements at the end
    void append(ConstSpan span) {
        ensureUnusedCapacity(span.count());
        Utils::copyConstruct(_vacantBegin(), span);
        m_count += span.count();
    }

    /// appends possibly multiple elements at the end
    void appendMoved(MoveSpan span) {
        ensureUnusedCapacity(span.count());
        Utils::moveConstruct(_vacantBegin(), span);
        m_count += span.count();
    }

    /// appends count default constructed elements
    void appendCount(Count count) {
        ensureUnusedCapacity(count);
        Utils::defaultConstruct(_amendVacant().head(count));
        m_count += count;
    }

    /// removes the last element
    void pop() {
        Utils::destruct(amend().tail(1));
        m_count--;
    }

    /// swiss army knife of mutating an dynamic array
    /// replaces:
    /// * insert - splice(it, 0, spanOfOne{V})
    /// * erase - splice(it, N, {})
    /// extensions:
    /// * insert of elements at once
    /// * combined remove and insert
    void splice(ConstIterator cIt, Count removeCount, ConstSpan insertSpan) {
        auto const offset = static_cast<Index>(cIt - begin());
        auto const it = _amendBegin() + offset;
        auto const insertCount = insertSpan.count();
        auto const remainCount = static_cast<Count>(m_count - offset - removeCount);
        auto const remainSpan = move().tail(remainCount);
        // old: [ ..(offset)..,[it] ..(removeCount).., ..(remainCount)... ]
        // new: [ ..(offset)..,[it] ..(insertCount).., ..(remainCount)... ]

        auto const newCount = static_cast<Count>(m_count - removeCount) + insertCount;
        if (m_capacity < newCount) { // not enough storage => arrange everything in new storage
            auto const newStorage = _grownStorage(insertCount - removeCount);
            Utils::moveConstruct(newStorage.begin(), move().head(offset));
            Utils::copyConstruct(newStorage.begin() + offset, insertSpan);
            Utils::moveConstruct(newStorage.begin() + offset + insertCount, remainSpan);
            Utils::destruct(amend());
            Utils::deallocate(_amendCapacity());
            m_data = newStorage.begin();
            m_capacity = newStorage.count();
        }
        else if (m_count >= newCount) { // shrinking
            auto const shrinkCount = static_cast<Count>(m_count - newCount);
            Utils::copyAssign(it, insertSpan);
            Utils::moveAssignForward(m_data + offset + insertCount, remainSpan);
            Utils::destruct(amend().tail(shrinkCount));
        }
        else if (offset + insertCount <= m_count) { // parts of remainSpan is moved beyond end()
            auto const growCount = static_cast<Count>(newCount - m_count);
            Utils::moveConstruct(_vacantBegin(), remainSpan.tail(growCount));
            Utils::moveAssignReverse(it + insertCount, remainSpan.drop_tail(growCount));
            Utils::copyAssign(it, insertSpan);
        }
        else { // remainSpan is moved beyond end()
            Utils::moveConstruct(m_data + offset + insertCount, remainSpan);
            auto const assignElems = static_cast<Count>(m_count - offset);
            Utils::copyAssign(it, insertSpan.head(assignElems));
            Utils::copyConstruct(_vacantBegin(), insertSpan.skip(assignElems));
        }
        m_count = newCount;
    }

    /// Same as splice but moves inserted elements into the array
    void spliceMoved(ConstIterator cIt, Count removeCount, MoveSpan insertSpan) {
        auto const offset = static_cast<Index>(cIt - begin());
        auto const it = _amendBegin() + offset;
        auto const insertCount = insertSpan.count();
        auto const remainCount = static_cast<Count>(m_count - offset - removeCount);
        auto const remainSpan = move().tail(remainCount);
        // old: [ ..(offset)..,[it] ..(removeCount).., ..(remainCount)... ]
        // new: [ ..(offset)..,[it] ..(insertCount).., ..(remainCount)... ]

        auto const newCount = static_cast<Count>(m_count - removeCount) + insertCount;
        if (m_capacity < newCount) { // not enough storage => arrange everything in new storage
            auto const newStorage = _grownStorage(insertCount - removeCount);
            Utils::moveConstruct(newStorage.begin(), move().head(offset));
            Utils::moveConstruct(newStorage.begin() + offset, insertSpan);
            Utils::moveConstruct(newStorage.begin() + offset + insertCount, remainSpan);
            Utils::destruct(amend());
            Utils::deallocate(_amendCapacity());
            m_data = newStorage.begin();
            m_capacity = newStorage.count();
        }
        else if (m_count >= newCount) { // shrinking
            auto const shrinkCount = static_cast<Count>(m_count - newCount);
            Utils::moveAssign(it, insertSpan);
            Utils::moveAssignForward(m_data + offset + insertCount, remainSpan);
            Utils::destruct(amend().tail(shrinkCount));
        }
        else if (offset + insertCount <= m_count) { // parts of remainSpan is moved beyond end()
            auto const growCount = static_cast<Count>(newCount - m_count);
            Utils::moveConstruct(_vacantBegin(), remainSpan.tail(growCount));
            Utils::moveAssignReverse(it + insertCount, remainSpan.drop_tail(growCount));
            Utils::moveAssign(it, insertSpan);
        }
        else { // remainSpan is moved beyond end()
            Utils::moveConstruct(m_data + offset + insertCount, remainSpan);
            auto const assignElems = static_cast<Count>(m_count - offset);
            Utils::moveAssign(it, insertSpan.head(assignElems));
            Utils::moveConstruct(_vacantBegin(), insertSpan.skip(assignElems));
        }
        m_count = newCount;
    }

    /// Same as splice but moves inserted elements into the array
    void remove(AmendIterator it, Count removeCount) {
        auto offset = it - _amendBegin();
        auto remainCount = m_count - offset - removeCount;
        auto remainSpan = MoveSpan{it + removeCount, remainCount};

        Utils::moveAssignForward(m_data + offset, remainSpan);
        Utils::destruct(AmendSpan{amend().end() - removeCount, removeCount});
        m_count = m_count - removeCount;
    }

private:
    [[nodiscard]] auto _amendBegin() -> AmendIterator { return std::launder(m_data); }
    [[nodiscard]] auto _vacantBegin() -> AmendIterator { return m_data + m_count; }
    [[nodiscard]] auto _amendCapacity() -> AmendSpan { return AmendSpan{m_data, m_capacity}; }
    [[nodiscard]] auto _amendVacant() -> AmendSpan { return _amendCapacity().skip(m_count); }

    [[nodiscard]] auto _grownStorage(size_t growBy) const -> AmendSpan {
        auto const cur = m_capacity;
        auto res = (cur << 1) - (cur >> 1) + (cur >> 4); // * 1.563
        if (res < 5) res = 5;
        if (res < m_capacity + growBy) res = m_capacity + growBy;
        auto ptr = Utils::allocate(res);
        return AmendSpan{ptr, res};
    }
    void _growBy(size_t by) {
        auto const newStorage = _grownStorage(by);
        Utils::moveConstruct(newStorage.begin(), move());
        Utils::destruct(amend());
        Utils::deallocate(AmendSpan{m_data, m_capacity});
        m_data = newStorage.begin();
        m_capacity = newStorage.count();
    }
};

/// simplified deduction guide
/// usage:
///     DynamicArray{1,2,3}
template<class T, class... Ts> DynamicArray(T&&, Ts&&...) -> DynamicArray<T>;

/// deduce T from span
template<class T> DynamicArray(Span<T const>) -> DynamicArray<T>;
template<class T> DynamicArray(Span<T>) -> DynamicArray<T>;

/// deduce Span from DynamicArray
/// usage:
///     auto a = DynamicArray{1,2,3;
///     auto span = Span{a};
template<class T> Span(const DynamicArray<T>&) -> Span<T const>;

} // namespace array19
