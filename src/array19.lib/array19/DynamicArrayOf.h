#pragma once
#include "SliceOf.h"

#include <new> // launder
#include <string.h> // memcpy

namespace array19 {

namespace details {

template<class ElemStorage> struct DynamicStorage final {
    using ElementStorage = ElemStorage;
    using Capacity = size_t;
    using Index = size_t;

    DynamicStorage() = default;
    ~DynamicStorage() noexcept { delete[] pointer; }

    DynamicStorage(DynamicStorage&& o) noexcept : pointer(o.pointer), capacity(o.capacity) { o.pointer = {}; }
    DynamicStorage& operator=(DynamicStorage&& o) noexcept {
        delete[] pointer;
        pointer = o.pointer;
        capacity = o.capacity;
        o.pointer = {};
        return *this;
    }

    [[nodiscard]] static auto create(Capacity capacity) -> DynamicStorage {
        auto result = DynamicStorage{};
        result.pointer = new ElementStorage[capacity];
        result.capacity = capacity;
        return result;
    }

    ElementStorage* pointer{};
    Capacity capacity{};
};

} // namespace details

template<class T> struct DynamicArrayOf final {
    using Element = T;
    using Count = size_t;
    using Index = size_t;

    using Reference = Element&;
    using ConstReference = const Element&;
    using Iterator = Element*;
    using ConstIterator = const Element*;
    using Slice = SliceOf<Element>;
    using ConstSlice = SliceOf<const Element>;

    DynamicArrayOf() = default;
    ~DynamicArrayOf() noexcept(std::is_nothrow_destructible_v<Element>) { destructSlice(amend()); }

    DynamicArrayOf(ConstSlice slice) { append(slice); }

    template<class... Ts>
    requires(sizeof...(Ts) > 0) DynamicArrayOf(Ts&&... args) noexcept(std::is_nothrow_copy_constructible_v<Element>)
            : m_storage(Storage::create(sizeof...(Ts)))
            , m_count(0) {
        (copyConstructSlice(m_storage.pointer + m_count++, ConstSlice{&T{args}, 1}), ...);
    }

    DynamicArrayOf(const DynamicArrayOf& o) noexcept(std::is_nothrow_copy_constructible_v<Element>)
            : m_storage(Storage::create(o.m_count))
            , m_count(o.m_count) {
        copyConstructSlice(m_storage.pointer, o);
    }
    DynamicArrayOf& operator=(const DynamicArrayOf& o) noexcept(std::is_nothrow_copy_assignable_v<Element>) {
        if (m_storage.capacity < o.m_count) {
            *this = DynamicArrayOf(o);
        }
        else {
            auto countDiff = m_count - o.m_count;
            if (countDiff > 0) {
                copyAssignSlice(amendBegin(), o.slice());
                destructSlice(Slice{amendBegin() + o.m_count, countDiff});
            }
            else {
                copyAssignSlice(amendBegin(), ConstSlice{o.begin(), m_count});
                copyConstructSlice(storageEnd(), ConstSlice{o.begin() + m_count, -countDiff});
            }
            m_count = o.m_count;
        }
    }

    DynamicArrayOf(DynamicArrayOf&& o) noexcept : m_storage(std::move(o.m_storage)), m_count(o.m_count) {
        o.m_count = 0;
    }
    DynamicArrayOf& operator=(DynamicArrayOf&& o) noexcept {
        destructSlice(amend());
        m_storage = std::move(o.m_storage);
        m_count = o.m_count;
        o.m_count = 0;
    }

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] auto count() const -> Count { return m_count; }
    [[nodiscard]] auto totalCapacity() const -> Count { return m_storage.capacity; }
    [[nodiscard]] auto unusedCapacity() const -> Count { return m_storage.capacity - m_count; }

    [[nodiscard]] auto begin() const noexcept -> ConstIterator {
        return std::launder(reinterpret_cast<const T*>(m_storage.pointer));
    }
    // NOTE: end() is not laundered!
    [[nodiscard]] auto end() const noexcept -> ConstIterator {
        return reinterpret_cast<const T*>(m_storage.pointer + m_count);
    }
    [[nodiscard]] auto operator[](Index index) const noexcept -> ConstReference {
        return *std::launder(reinterpret_cast<const T*>(m_storage.pointer + index));
    }
    [[nodiscard]] operator ConstSlice() const noexcept { return ConstSlice{begin(), m_count}; }

    [[nodiscard]] auto amendBegin() & noexcept -> Iterator {
        return std::launder(reinterpret_cast<T*>(m_storage.pointer));
    }
    // NOTE: end() is not laundered!
    [[nodiscard]] auto amendEnd() & noexcept -> Iterator { return amendBegin() + m_count; }

    [[nodiscard]] auto amend() -> Slice { return Slice{amendBegin(), m_count}; }

    void ensureCapacity(Count count) {
        if (totalCapacity() < count) growBy(count - totalCapacity());
    }
    void ensureUnusedCapacity(Count count) {
        if (unusedCapacity() < count) growBy(count);
    }

    template<class... Ts> void emplace_back(Ts&&... args) {
        ensureUnusedCapacity(1);
        new (m_storage.pointer + m_count) Element((Ts &&) args...);
        m_count++;
    }

    void append(ConstSlice elems) {
        ensureUnusedCapacity(elems.count());
        copyConstructSlice(storageEnd(), elems);
        m_count += elems.count();
    }

    void pop() noexcept(std::is_nothrow_destructible_v<Element>) {
        destructSlice(Slice{amendBegin() + m_count - 1, 1});
        m_count--;
    }

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
            moveConstructSlice(newStorage.pointer, Slice{amendBegin(), static_cast<size_t>(offset)});
            copyConstructSlice(newStorage.pointer + offset, insertSlice);
            moveConstructSlice(newStorage.pointer + offset + insertCount, remainSlice);
            destructSlice(amend());
            m_storage = std::move(newStorage);
        }
        else if (countDiff <= 0) { // shrinking
            copyAssignSlice(it, insertSlice);
            forwardMoveConstructSlice(m_storage.pointer + offset + insertCount, remainSlice);
            destructSlice(Slice{amendEnd() + countDiff, -countDiff});
        }
        else if (offset + insertCount <= m_count) { // parts of remainSlice is moved beyond end()
            moveConstructSlice(storageEnd(), remainSlice.slice(remainCount - countDiff, countDiff));
            reverseMoveAssignSlice(it + insertCount, remainSlice.slice(0, remainCount - countDiff));
            copyAssignSlice(it, insertSlice);
        }
        else { // remainSlice is moved beyond end()
            moveConstructSlice(m_storage.pointer + offset + insertCount, remainSlice);
            auto assignElems = m_count - offset;
            copyAssignSlice(it, insertSlice.slice(0, assignElems));
            copyConstructSlice(storageEnd(), insertSlice.slice(assignElems, insertCount - assignElems));
        }
        m_count += countDiff;
    }

private:
    struct ElementStorage final {
        alignas(T) unsigned char data[sizeof(T)];
    };
    using Storage = details::DynamicStorage<ElementStorage>;
    using StorageSlice = SliceOf<ElementStorage>;
    using StorageIterator = ElementStorage*;
    constexpr static auto element_size = sizeof(ElementStorage);

    [[nodiscard]] auto byteSize() const -> size_t { return m_count * element_size; }
    [[nodiscard]] auto storageEnd() -> StorageIterator { return m_storage.pointer + m_count; }

    static void destructSlice(Slice slice) {
        for (auto& elem : slice) elem.~Element();
    }
    static void copyAssignSlice(Iterator to, ConstSlice fromSlice) {
        if constexpr (std::is_trivially_copy_assignable_v<Element>) {
            memcpy(to, fromSlice.begin(), fromSlice.count() * element_size);
        }
        else {
            for (const auto& e : fromSlice) *to++ = e;
        }
    }
    static void moveAssignSlice(Iterator to, Slice fromSlice) {
        if constexpr (std::is_trivially_move_assignable_v<Element>) {
            memcpy(to, fromSlice.begin(), fromSlice.count() * element_size);
        }
        else {
            for (const auto& e : fromSlice) *to++ = std::move(e);
        }
    }
    static void reverseMoveAssignSlice(Iterator to, Slice fromSlice) {
        if constexpr (std::is_trivially_move_assignable_v<Element>) {
            memmove(to, fromSlice.begin(), fromSlice.count() * element_size);
        }
        else {
            auto rTo = to + fromSlice.count();
            auto rFrom = fromSlice.end();
            auto rFromEnd = fromSlice.begin();
            while (rFrom != rFromEnd) *(--rTo) = std::move(*(--rFrom));
        }
    }
    static void copyConstructSlice(StorageIterator to, ConstSlice fromSlice) {
        if constexpr (std::is_trivially_copy_constructible_v<Element>) {
            memcpy(to, fromSlice.begin(), fromSlice.count() * element_size);
        }
        else {
            for (auto& from : fromSlice) new (to++) Element(from);
        }
    }
    static void moveConstructSlice(StorageIterator to, Slice fromSlice) {
        if constexpr (std::is_trivially_move_constructible_v<Element>) {
            memcpy(to, fromSlice.begin(), fromSlice.count() * element_size);
        }
        else {
            for (auto& from : fromSlice) new (to++) Element(std::move(from));
        }
    }
    static void forwardMoveConstructSlice(StorageIterator to, Slice fromSlice) {
        if constexpr (std::is_trivially_move_constructible_v<Element>) {
            memmove(to, fromSlice.begin(), fromSlice.count() * element_size);
        }
        else {
            for (auto& from : fromSlice) new (to++) Element(std::move(from));
        }
    }
    [[nodiscard]] auto grownStorage(int growBy) const -> Storage {
        auto cur = totalCapacity();
        auto res = (cur << 1) - (cur >> 1) + (cur >> 4); // * 1.563
        if (res < 5) res = 5;
        if (res < totalCapacity() + growBy) res = totalCapacity() + growBy;
        return Storage::create(res);
    }
    void growBy(int by) {
        auto newStorage = grownStorage(by);
        moveConstructSlice(newStorage.pointer, amend());
        destructSlice(amend());
        m_storage = std::move(newStorage);
    }

private:
    Storage m_storage;
    Count m_count;
};

} // namespace array19
