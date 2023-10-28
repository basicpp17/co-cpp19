#pragma once
#include "OrderedSliceOf.h"
#include "array19/AllocatedArrayUtils.h"
#include "array19/MoveSliceOf.h"

namespace lookup19 {

using array19::MoveSliceOf;

struct DefaultLess {
    template<class A, class B> constexpr auto operator()(const A& a, const B& b) const { return a < b; }
};

/// Stores an dynamic allocated array that stores keys before values
/// notes:
/// * avoids pointer chasing
/// * addresses are not stable on insert or remove
/// * prefer to store value pointers for big objects to make efficient
template<class Key, class Value, class Less = DefaultLess> struct OrderedMapOf {
    using Count = size_t;
    using Index = size_t;

    using KeySlice = SliceOf<Key const>;
    using KeyOrderedSlice = OrderedSliceOf<const Key, Less>;
    using KeyMoveSlice = MoveSliceOf<Key>;
    using KeyAmendSlice = SliceOf<Key>;

    using ValueSlice = SliceOf<Value const>;
    using ValueAmendSlice = SliceOf<Value>;
    using ValueMoveSlice = MoveSliceOf<Value>;

private:
    using KeyUtils = array19::AllocatedArrayUtils<Key>;
    using ValueUtils = array19::AllocatedArrayUtils<Value>;

    static constexpr auto value_align_offset = alignof(Key) < alignof(Value) ? alignof(Value) - alignof(Key) : 0;
    static constexpr auto value_align_mask = ~static_cast<uintptr_t>(alignof(Value) - 1);

    Key* m_pointer{};
    Count m_count{};
    Count m_capacity{};

    [[nodiscard]] auto valuesPointer() -> Value* { return toValuesPointer(m_pointer + m_capacity); }
    [[nodiscard]] auto valuesPointer() const -> Value const* { return toValuesPointer(m_pointer + m_capacity); }

    [[nodiscard]] auto keyData() const noexcept -> Key const* {
        return std::launder(reinterpret_cast<const Key*>(m_pointer));
    }
    [[nodiscard]] auto valueData() noexcept -> Value* {
        return std::launder(reinterpret_cast<Value*>(valuesPointer()));
    }
    [[nodiscard]] auto valueData() const noexcept -> Value const* {
        return std::launder(reinterpret_cast<const Value*>(valuesPointer()));
    }

public:
    OrderedMapOf() = default;
    ~OrderedMapOf() noexcept {
        if (m_pointer) {
            KeyUtils::destruct(KeyAmendSlice{m_pointer, m_count});
            ValueUtils::destruct(ValueAmendSlice{valuesPointer(), m_count});
            OrderedMapOf::deallocate(KeyAmendSlice{m_pointer, m_capacity});
        }
    }

    explicit OrderedMapOf(KeyOrderedSlice newKeys, Value const* newValues)
            : m_pointer{OrderedMapOf::allocate(newKeys.count())}
            , m_count{newKeys.count()}
            , m_capacity{newKeys.count()} {
        KeyUtils::copyConstruct(m_pointer, newKeys);
        ValueUtils::copyConstruct(valuesPointer(), ValueSlice{newValues, newKeys.count()});
    }

    OrderedMapOf(const OrderedMapOf& o)
            : m_pointer{OrderedMapOf::allocate(o.m_count)}
            , m_count(o.m_count)
            , m_capacity(o.m_count) {
        KeyUtils::copyConstruct(m_pointer, o.keys());
        ValueUtils::copyConstruct(valuesPointer(), o.values());
    }
    OrderedMapOf& operator=(const OrderedMapOf& o) {
        if (!m_pointer || m_capacity < o.m_count) {
            *this = OrderedMapOf{o};
        }
        else {
            if (m_count > o.m_count) {
                KeyUtils::copyAssign(m_pointer, o.keys());
                ValueUtils::copyAssign(valuesPointer(), o.values());
                KeyUtils::destruct(KeyAmendSlice{m_pointer + o.m_count, m_count - o.m_count});
                ValueUtils::destruct(ValueAmendSlice{valuesPointer() + o.m_count, m_count - o.m_count});
            }
            else {
                KeyUtils::copyAssign(m_pointer, KeySlice{o.keyData(), m_count});
                ValueUtils::copyAssign(valuesPointer(), ValueSlice{o.valueData(), m_count});
                KeyUtils::copyConstruct(m_pointer + m_count, KeySlice{o.keyData() + m_count, o.m_count - m_count});
                ValueUtils::copyConstruct(
                    valuesPointer() + m_count,
                    ValueSlice{o.valueData() + m_count, o.m_count - m_count});
            }
            m_count = o.m_count;
        }
        return *this;
    }

    OrderedMapOf(OrderedMapOf&& o) noexcept : m_pointer(o.m_pointer), m_count(o.m_count), m_capacity(o.m_capacity) {
        o.m_pointer = nullptr;
    }
    OrderedMapOf& operator=(OrderedMapOf&& o) noexcept {
        if (m_pointer) {
            return *this = OrderedMapOf{std::move(o)};
        }
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

    [[nodiscard]] auto keys() const noexcept -> KeySlice { return KeySlice{keyData(), m_count}; }
    [[nodiscard]] auto orderedKeys() const noexcept -> KeyOrderedSlice { return KeyOrderedSlice{keyData(), m_count}; }
    // note: amendKeys is omitted, to ensure order of elements is preserved!
    [[nodiscard]] auto moveKeys() noexcept -> KeyMoveSlice { return KeyMoveSlice{m_pointer, m_count}; }

    [[nodiscard]] auto values() const noexcept -> ValueSlice { return ValueSlice{valueData(), m_count}; }
    [[nodiscard]] auto amendValues() noexcept -> ValueAmendSlice { return ValueAmendSlice{valueData(), m_count}; }
    [[nodiscard]] auto moveValues() noexcept -> ValueMoveSlice { return ValueMoveSlice{valueData(), m_count}; }

    void ensureCapacity(Count count) {
        if (totalCapacity() < count) growBy(count - totalCapacity());
    }
    void ensureUnusedCapacity(Count count) {
        if (unusedCapacity() < count) growBy(count);
    }

    [[nodiscard]] bool hasKey(Key const& key) const {
        auto keyRange = orderedKeys().equalRange(key);
        return !keyRange.isEmpty();
    }

    [[nodiscard]] auto findBy(Key const& key) const noexcept -> Value const* {
        auto keyRange = orderedKeys().equalRange(key);
        if (keyRange.isEmpty()) return nullptr;
        auto index = static_cast<Index>(keyRange.begin() - m_pointer);
        return valueData() + index;
    }
    [[nodiscard]] auto amendBy(Key const& key) noexcept -> Value* {
        auto keyRange = orderedKeys().equalRange(key);
        if (keyRange.isEmpty()) return nullptr;
        auto index = static_cast<Index>(keyRange.begin() - m_pointer);
        return valueData() + index;
    }

    /// emplace a single value to the map if key is not present
    /// returns nullptr if nothing changed
    template<class... Args> auto emplace(Key key, Args&&... args) -> Value* {
        auto valueIt = add(std::move(key));
        if (valueIt == nullptr) return nullptr;
        return new (valueIt) Value((Args &&) args...);
    }

    /// inserts a single value to the map if key is not present
    auto insert(Key key, Value value) -> Value* {
        auto valueIt = add(std::move(key));
        if (valueIt == nullptr) return nullptr;
        ValueUtils::moveConstruct(valueIt, ValueMoveSlice{&value, 1});
        return valueIt;
    }

    void mergeRange(KeyOrderedSlice newKeys, Value const* newValues) {
        if (newKeys.isEmpty()) return;
        if (isEmpty()) {
            *this = OrderedMapOf{newKeys, newValues};
            return;
        }
        auto const totalCount = newKeys.count() + m_count;
        if (totalCount > m_capacity) {
            auto newStorage = grownStorage(newKeys.count());
            auto count = mergeConstructInto(newStorage, newKeys, newValues);
            KeyUtils::destruct(KeyAmendSlice{m_pointer, m_count});
            ValueUtils::destruct(ValueAmendSlice{valuesPointer(), m_count});
            deallocate(KeyAmendSlice{m_pointer, m_capacity});
            m_pointer = newStorage.begin();
            m_count = count;
            m_capacity = newStorage.count();
        }
        else {
            m_count = inplaceMerge(newKeys, newValues);
        }
    }

    void remove(Key const* cIt) {
        auto keyIt = const_cast<Key*>(cIt);
        auto index = static_cast<Index>(keyIt - m_pointer);
        auto valueIt = valuesPointer() + index;
        KeyUtils::destruct(KeyAmendSlice{keyIt, 1});
        ValueUtils::destruct(ValueAmendSlice{valueIt, 1});
        if (index + 1 != m_count) {
            KeyUtils::moveAssignReverse(keyIt, KeyMoveSlice{keyIt + 1, m_count - index - 1});
            ValueUtils::moveAssignReverse(valueIt, ValueMoveSlice{valueIt + 1, m_count - index - 1});
        }
        m_count--;
    }

private:
    static auto allocate(size_t count) -> Key* {
        if (count == 0) return nullptr;
        auto key_size = sizeof(Key) * count;
        auto aligned_size = (key_size + value_align_offset) & value_align_mask;
        auto size = aligned_size + sizeof(Value) * count;
        if constexpr (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(Key)) {
            return reinterpret_cast<Key*>(::operator new[](size, std::align_val_t{alignof(Key)}));
        }
        else {
            return reinterpret_cast<Key*>(::operator new[](size));
        }
    }
    static auto deallocate(KeyAmendSlice slice) {
#if __cpp_sized_deallocation
        auto key_size = sizeof(Key) * slice.count();
        auto aligned_size = (key_size + value_align_offset) & value_align_mask;
        auto size = aligned_size + sizeof(Value) * slice.count();
        if constexpr (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(Key)) {
            ::operator delete[](slice.begin(), size, std::align_val_t{alignof(Key)});
        }
        else {
            ::operator delete[](slice.begin(), size);
        }
#else
        if constexpr (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(Key)) {
            ::operator delete[](slice.begin(), std::align_val_t{alignof(Key)});
        }
        else {
            ::operator delete[](slice.begin());
        }
#endif
    }
    template<class Key, class Value = std::conditional_t<std::is_const_v<Key>, Value const, Value>>
    static auto toValuesPointer(Key* ptr) -> Value* {
        auto aligned_address = (std::bit_cast<uintptr_t>(ptr) + value_align_offset) & value_align_mask;
        return std::bit_cast<Value*>(aligned_address);
    }

    auto keyToValue(Key const* key) -> Value& { return valuesPointer()[key - m_pointer]; }
    auto keyToValue(Key const* key) const -> Value const& { return valuesPointer()[key - m_pointer]; }

    [[nodiscard]] auto grownStorage(int growBy) const -> KeyAmendSlice {
        auto cur = m_capacity;
        auto res = (cur << 1) - (cur >> 1) + (cur >> 4); // * 1.563
        if (res < 5) res = 5;
        if (res < m_capacity + growBy) res = m_capacity + growBy;
        auto ptr = OrderedMapOf::allocate(res);
        return KeyAmendSlice{ptr, res};
    }
    void growBy(int by) {
        auto newStorage = grownStorage(by);
        KeyUtils::moveConstruct(newStorage.begin(), moveKeys());
        ValueUtils::moveConstruct(toValuesPointer(newStorage.end()), moveValues());
        KeyUtils::destruct(KeyAmendSlice{m_pointer, m_count});
        ValueUtils::destruct(amendValues());
        OrderedMapOf::deallocate(SliceOf{m_pointer, m_capacity});
        m_pointer = newStorage.begin();
        m_capacity = newStorage.count();
    }

    auto add(Key key) -> Value* {
        if (m_count == m_capacity) {
            return addWithGrowing(key);
        }
        return addToCapacity(key);
    }
    auto addToCapacity(Key key) -> Value* {
        auto keyIt = const_cast<Key*>(orderedKeys().lowerBound(key));
        if (keyIt != orderedKeys().end() && *keyIt == key) {
            return nullptr;
        }
        auto index = static_cast<Index>(keyIt - m_pointer);
        auto valueIt = valuesPointer() + index;
        if (index != m_count) {
            KeyUtils::moveAssignForward(keyIt + 1, KeyMoveSlice{keyIt, m_count - index});
            ValueUtils::moveAssignForward(valueIt + 1, ValueMoveSlice{valueIt, m_count - index});
        }
        KeyUtils::moveAssign(keyIt, KeyMoveSlice{&key, 1});
        m_count++;
        return valueIt;
    }
    auto addWithGrowing(Key key) -> Value* {
        auto keyIt = const_cast<Key*>(orderedKeys().lowerBound(key));
        if (keyIt != orderedKeys().end() && *keyIt == key) {
            return nullptr;
        }
        auto newStorage = grownStorage(1);
        auto nKeyPtr = newStorage.begin();
        auto nValuePtr = toValuesPointer(newStorage.end());
        auto index = static_cast<Index>(keyIt - m_pointer);
        if (0 != index) {
            KeyUtils::moveConstruct(nKeyPtr, KeyMoveSlice{m_pointer, index});
            ValueUtils::moveConstruct(nValuePtr, ValueMoveSlice{valuesPointer(), index});
            nKeyPtr += index;
            nValuePtr += index;
        }
        KeyUtils::moveConstruct(nKeyPtr, KeyMoveSlice{&key, 1});
        // note: value is added outside
        if (index != m_count) {
            KeyUtils::moveConstruct(nKeyPtr + 1, KeyMoveSlice{m_pointer + index, m_count - index});
            ValueUtils::moveConstruct(nValuePtr + 1, ValueMoveSlice{valuesPointer() + index, m_count - index});
        }
        deallocate(KeyAmendSlice{m_pointer, m_capacity});
        m_pointer = newStorage.begin();
        m_capacity = newStorage.count();
        m_count++;
        return nValuePtr;
    }

    auto inplaceMerge(KeyOrderedSlice newKeys, Value const* newValues) -> Index {
        auto const less = Less{};
        auto totalCount = newKeys.count() + m_count;
        auto const keyPtr = m_pointer;
        auto const valuePtr = valuesPointer();
        auto const oldBegin = m_pointer - 1;
        auto oldIt = oldBegin + m_count;
        auto const newBegin = newKeys.begin() - 1;
        auto newIt = newBegin + newKeys.count();
        auto outIndex = Index{totalCount};
        auto equalCount = Index{};
        auto const finishNew = [&]() {
            auto newCount = static_cast<Index>(newIt - newBegin);
            if (outIndex >= m_count) {
                auto constructCount = outIndex - m_count;
                outIndex = m_count;
                newCount -= constructCount;
                KeyUtils::copyConstruct(keyPtr + outIndex, KeySlice{newBegin + 1 + newCount, constructCount});
                ValueUtils::copyConstruct(valuePtr + outIndex, ValueSlice{newValues + newCount, constructCount});
            }
            if (newCount > 0) {
                outIndex -= newCount;
                KeyUtils::copyAssign(keyPtr + outIndex, KeySlice{newBegin + 1, newCount});
                ValueUtils::copyAssign(valuePtr + outIndex, ValueSlice{newValues, newCount});
            }
        };
        while (true) {
            if (less(*newIt, *oldIt)) {
                auto oldB = oldIt - 1;
                while (oldB != oldBegin && less(*newIt, *oldB)) oldB -= 1;
                auto count = static_cast<Index>(oldIt - oldB);
                if (outIndex >= m_count) {
                    auto constructCount = count;
                    outIndex -= count;
                    if (outIndex < m_count) {
                        constructCount -= m_count - outIndex;
                        outIndex = m_count;
                    }
                    count -= constructCount;
                    KeyUtils::moveConstruct(keyPtr + outIndex, KeyMoveSlice{oldB + 1 + count, constructCount});
                    ValueUtils::moveConstruct(
                        valuePtr + outIndex,
                        ValueMoveSlice{valuePtr + (oldB - oldBegin + count), constructCount});
                }
                if (count > 0) {
                    outIndex -= count;
                    KeyUtils::moveAssignForward(keyPtr + outIndex, KeyMoveSlice{oldB + 1, count});
                    ValueUtils::moveAssignForward(
                        valuePtr + outIndex,
                        ValueMoveSlice{valuePtr + (oldB - oldBegin), count});
                }
                oldIt = oldB;
                if (oldIt == oldBegin) {
                    finishNew();
                    break;
                }
            }
            if (!less(*oldIt, *newIt)) {
                equalCount += 1;
                oldIt -= 1;
                if (oldIt == oldBegin) {
                    finishNew();
                    break;
                }
            }
            {
                auto newB = newIt - 1;
                while (newB != newBegin && less(*oldIt, *newB)) newB -= 1;
                auto count = static_cast<Index>(newIt - newB);
                if (outIndex >= m_count) {
                    auto constructCount = count;
                    outIndex -= count;
                    if (outIndex < m_count) {
                        constructCount -= m_count - outIndex;
                        outIndex = m_count;
                    }
                    count -= constructCount;
                    KeyUtils::copyConstruct(keyPtr + outIndex, KeySlice{newB + 1 + count, constructCount});
                    ValueUtils::copyConstruct(
                        valuePtr + outIndex,
                        ValueSlice{newValues + (newB - newBegin) + count, constructCount});
                }
                if (count > 0) {
                    outIndex -= count;
                    KeyUtils::copyAssign(keyPtr + outIndex, KeySlice{newB + 1, count});
                    ValueUtils::copyAssign(valuePtr + outIndex, ValueSlice{newValues + (newB - newBegin), count});
                }
                newIt = newB;
                if (newIt == newBegin) {
                    break; // remaining old values stay place
                }
            }
        }
        if (equalCount > 0) {
            totalCount -= equalCount;
            auto offset = static_cast<Index>(oldIt - oldBegin);
            KeyUtils::moveAssignReverse(
                keyPtr + offset,
                KeyMoveSlice{keyPtr + offset + equalCount, totalCount - offset});
            ValueUtils::moveAssignReverse(
                valuePtr + offset,
                ValueMoveSlice{valuePtr + offset + equalCount, totalCount - offset});
        }
        return totalCount;
    }
    auto mergeConstructInto(KeyAmendSlice newStorage, KeyOrderedSlice newKeys, Value const* newValues) -> Index {
        auto const less = Less{};
        auto const keyPtr = newStorage.begin();
        auto const valuePtr = toValuesPointer(newStorage.end());
        auto const oldBegin = m_pointer;
        auto const oldEnd = oldBegin + m_count;
        auto const oldValuePtr = valuesPointer();
        auto const newBegin = newKeys.begin();
        auto const newEnd = newKeys.end();
        auto oldIt = oldBegin;
        auto newIt = newBegin;
        auto totalCount = Index{};
        auto finishNew = [&]() {
            auto newCount = static_cast<Index>(newEnd - newIt);
            KeyUtils::copyConstruct(keyPtr + totalCount, KeySlice{newIt, newCount});
            ValueUtils::copyConstruct(valuePtr + totalCount, ValueSlice{newValues + (newIt - newBegin), newCount});
            totalCount += newCount;
        };
        auto finishOld = [&]() {
            auto oldCount = static_cast<Index>(oldEnd - oldIt);
            KeyUtils::moveConstruct(keyPtr + totalCount, KeyMoveSlice{oldIt, oldCount});
            ValueUtils::moveConstruct(
                valuePtr + totalCount,
                ValueMoveSlice{oldValuePtr + (oldIt - oldBegin), oldCount});
            totalCount += oldCount;
        };

        while (true) {
            if (!less(*oldIt, *newIt)) {
                auto newE = newIt;
                do {
                    if (!less(*newE, *oldIt)) {
                        oldIt += 1;
                        if (oldIt == oldEnd) {
                            finishNew();
                            return totalCount;
                        }
                    }
                    newE += 1;
                } while (newE != newEnd && !less(*oldIt, *newE));
                auto count = static_cast<Index>(newE - newIt);
                KeyUtils::copyConstruct(keyPtr + totalCount, KeySlice{newIt, count});
                ValueUtils::copyConstruct(valuePtr + totalCount, ValueSlice{newValues + (newIt - newBegin), count});
                totalCount += count;
                newIt = newE;
                if (newIt == newEnd) {
                    finishOld();
                    return totalCount;
                }
            }
            {
                auto oldE = oldIt + 1;
                while (oldE != oldEnd && less(*oldE, *newIt)) oldE += 1;
                auto count = static_cast<Index>(oldE - oldIt);
                KeyUtils::moveConstruct(keyPtr + totalCount, KeyMoveSlice{oldIt, count});
                ValueUtils::moveConstruct(
                    valuePtr + totalCount,
                    ValueMoveSlice{oldValuePtr + (oldIt - oldBegin), count});
                totalCount += count;
                oldIt = oldE;
                if (oldIt == oldEnd) {
                    finishNew();
                    return totalCount;
                }
            }
        }
    }
};

} // namespace lookup19
