#pragma once
#include "OrderedSpan.h"
#include "array19/AllocatedArrayUtils.h"
#include "array19/Span.one.h"

#include <memory> // uintptr_t
#include <bit> // std::bit_cast

namespace lookup19 {

struct DefaultLess {
    template<class A, class B> constexpr auto operator()(A const& a, B const& b) const -> bool { return a < b; }
};

/// Stores an dynamic allocated array that stores keys before values
/// notes:
/// * avoids pointer chasing
/// * addresses are not stable on insert or remove
/// * prefer to store value pointers for big objects to make efficient
template<class K, class V, class L = DefaultLess> struct OrderedMapArray {
    using Key = K;
    using Value = V;
    using Less = L;
    using Count = size_t;
    using Index = size_t;

    using KeyConstSpan = Span<Key const>;
    using KeyAmendSpan = Span<Key>;
    using KeyMoveSpan = Span<Key&&>;
    using KeyConstOrderedSpan = OrderedSpan<const Key, Less>;
    using KeyAmendOrderedSpan = OrderedSpan<Key, Less>;
    using KeyMoveOrderedSpan = OrderedSpan<Key&&, Less>;

    using ValueConstSpan = Span<Value const>;
    using ValueAmendSpan = Span<Value>;
    using ValueMoveSpan = Span<Value&&>;

private:
    using KeyUtils = array19::AllocatedArrayUtils<Key>;
    using ValueUtils = array19::AllocatedArrayUtils<Value>;

    static constexpr auto value_align_offset = alignof(Key) < alignof(Value) ? alignof(Value) - alignof(Key) : 0;
    static constexpr auto value_align_mask = ~static_cast<uintptr_t>(alignof(Value) - 1);

    Key* m_keyData{};
    Count m_count{};
    Count m_capacity{};

    [[nodiscard]] auto _valueData() -> Value* { return _toValuesPointer(m_keyData + m_capacity); }
    [[nodiscard]] auto _valueData() const -> Value const* { return _toValuesPointer(m_keyData + m_capacity); }

    [[nodiscard]] auto _keyConstBegin() const noexcept -> Key const* {
        return std::launder(reinterpret_cast<const Key*>(m_keyData));
    }
    [[nodiscard]] auto _keyAmendBegin() const noexcept -> Key* {
        return std::launder(reinterpret_cast<Key*>(m_keyData));
    }
    [[nodiscard]] auto _keyAmend() const noexcept -> KeyAmendSpan { return KeyAmendSpan{_keyAmendBegin(), m_count}; }
    [[nodiscard]] auto _valueAmendBegin() noexcept -> Value* {
        return std::launder(reinterpret_cast<Value*>(_valueData()));
    }
    [[nodiscard]] auto _valueConstBegin() const noexcept -> Value const* {
        return std::launder(reinterpret_cast<const Value*>(_valueData()));
    }
    [[nodiscard]] auto _amendCapacity() const noexcept -> KeyAmendSpan { return KeyAmendSpan{m_keyData, m_capacity}; }

public:
    OrderedMapArray() = default;
    ~OrderedMapArray() noexcept {
        if (m_keyData) {
            KeyUtils::destruct(_keyAmend());
            ValueUtils::destruct(amendValues());
            OrderedMapArray::deallocate(_amendCapacity());
        }
    }

    explicit OrderedMapArray(KeyConstOrderedSpan newKeys, Value const* newValues)
            : m_keyData{OrderedMapArray::allocate(newKeys.count())}
            , m_count{newKeys.count()}
            , m_capacity{newKeys.count()} {
        KeyUtils::copyConstruct(m_keyData, newKeys);
        ValueUtils::copyConstruct(_valueData(), ValueConstSpan{newValues, newKeys.count()});
    }
    explicit OrderedMapArray(KeyMoveOrderedSpan newKeys, Value* newValues)
            : m_keyData{OrderedMapArray::allocate(newKeys.count())}
            , m_count{newKeys.count()}
            , m_capacity{newKeys.count()} {
        KeyUtils::moveConstruct(m_keyData, newKeys);
        ValueUtils::moveConstruct(_valueData(), ValueMoveSpan{newValues, newKeys.count()});
    }

    OrderedMapArray(OrderedMapArray const& o)
            : m_keyData{OrderedMapArray::allocate(o.m_count)}
            , m_count(o.m_count)
            , m_capacity(o.m_count) {
        KeyUtils::copyConstruct(m_keyData, o.keys());
        ValueUtils::copyConstruct(_valueData(), o.values());
    }
    OrderedMapArray& operator=(OrderedMapArray const& o) {
        if (!m_keyData || m_capacity < o.m_count) {
            *this = OrderedMapArray{o};
        }
        else {
            if (m_count > o.m_count) {
                KeyUtils::copyAssign(m_keyData, o.keys());
                ValueUtils::copyAssign(_valueData(), o.values());
                KeyUtils::destruct(_keyAmend().skip(o.m_count));
                ValueUtils::destruct(amendValues().skip(o.m_count));
            }
            else {
                KeyUtils::copyAssign(m_keyData, o.keys().head(m_count));
                ValueUtils::copyAssign(_valueData(), o.values().head(m_count));
                KeyUtils::copyConstruct(m_keyData + m_count, o.keys().skip(m_count));
                ValueUtils::copyConstruct(_valueData() + m_count, o.values().skip(m_count));
            }
            m_count = o.m_count;
        }
        return *this;
    }

    OrderedMapArray(OrderedMapArray&& o) noexcept
            : m_keyData(std::exchange(o.m_keyData, nullptr))
            , m_count(o.m_count)
            , m_capacity(o.m_capacity) {}
    OrderedMapArray& operator=(OrderedMapArray&& o) noexcept {
        if (m_keyData) {
            KeyUtils::destruct(_keyAmend());
            ValueUtils::destruct(amendValues());
            OrderedMapArray::deallocate(_amendCapacity());
        }
        m_keyData = std::exchange(o.m_keyData, nullptr);
        m_count = o.m_count;
        m_capacity = o.m_capacity;
        return *this;
    }

    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return m_count == 0; }
    [[nodiscard]] auto count() const -> Count { return m_count; }
    [[nodiscard]] auto totalCapacity() const -> Count { return m_capacity; }
    [[nodiscard]] auto unusedCapacity() const -> Count { return m_capacity - m_count; }

    [[nodiscard]] auto keys() const noexcept -> KeyConstSpan { return KeyConstSpan{_keyConstBegin(), m_count}; }
    [[nodiscard]] auto orderedKeys() const noexcept -> KeyConstOrderedSpan {
        return KeyConstOrderedSpan{_keyConstBegin(), m_count};
    }
    // note: amendKeys is omitted, to ensure order of elements is preserved!
    [[nodiscard]] auto moveKeys() noexcept -> KeyMoveSpan { return KeyMoveSpan{_keyAmendBegin(), m_count}; }

    [[nodiscard]] auto values() const noexcept -> ValueConstSpan { return ValueConstSpan{_valueConstBegin(), m_count}; }
    [[nodiscard]] auto amendValues() noexcept -> ValueAmendSpan { return ValueAmendSpan{_valueAmendBegin(), m_count}; }
    [[nodiscard]] auto moveValues() noexcept -> ValueMoveSpan { return ValueMoveSpan{_valueAmendBegin(), m_count}; }

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
        auto index = static_cast<Index>(keyRange.begin() - m_keyData);
        return _valueConstBegin() + index;
    }
    [[nodiscard]] auto amendBy(Key const& key) noexcept -> Value* {
        auto keyRange = orderedKeys().equalRange(key);
        if (keyRange.isEmpty()) return nullptr;
        auto index = static_cast<Index>(keyRange.begin() - m_keyData);
        return _valueAmendBegin() + index;
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
        ValueUtils::moveConstruct(valueIt, ValueMoveSpan{&value, 1});
        return valueIt;
    }

    void mergeRange(KeyConstOrderedSpan newKeys, Value const* newValues) {
        if (newKeys.isEmpty()) return;
        if (isEmpty()) {
            *this = OrderedMapArray{newKeys, newValues};
            return;
        }
        auto const totalCount = newKeys.count() + m_count;
        if (totalCount > m_capacity) {
            auto newStorage = grownStorage(newKeys.count());
            auto count = mergeConstructInto(newStorage, newKeys, newValues);
            KeyUtils::destruct(KeyAmendSpan{m_keyData, m_count});
            ValueUtils::destruct(ValueAmendSpan{_valueData(), m_count});
            deallocate(KeyAmendSpan{m_keyData, m_capacity});
            m_keyData = newStorage.begin();
            m_count = count;
            m_capacity = newStorage.count();
        }
        else {
            m_count = inplaceMerge(newKeys, newValues);
        }
    }
    void mergeMoveRange(KeyMoveOrderedSpan newKeys, Value* newValues) {
        if (newKeys.isEmpty()) return;
        if (isEmpty()) {
            *this = OrderedMapArray{newKeys, newValues};
            return;
        }
        // TODO
    }

    void remove(Key const* cIt) {
        auto keyIt = const_cast<Key*>(cIt);
        auto index = static_cast<Index>(keyIt - m_keyData);
        auto valueIt = _valueData() + index;
        KeyUtils::destruct(KeyAmendSpan{keyIt, 1});
        ValueUtils::destruct(ValueAmendSpan{valueIt, 1});
        if (index + 1 != m_count) {
            KeyUtils::moveAssignReverse(keyIt, moveKeys().skip(index + 1));
            ValueUtils::moveAssignReverse(valueIt, moveValues().skip(index + 1));
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
    static auto deallocate(KeyAmendSpan span) {
#if __cpp_sized_deallocation
        auto key_size = sizeof(Key) * span.count();
        auto aligned_size = (key_size + value_align_offset) & value_align_mask;
        auto size = aligned_size + sizeof(Value) * span.count();
        if constexpr (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(Key)) {
            ::operator delete[](span.begin(), size, std::align_val_t{alignof(Key)});
        }
        else {
            ::operator delete[](span.begin(), size);
        }
#else
        if constexpr (__STDCPP_DEFAULT_NEW_ALIGNMENT__ < alignof(Key)) {
            ::operator delete[](span.begin(), std::align_val_t{alignof(Key)});
        }
        else {
            ::operator delete[](span.begin());
        }
#endif
    }
    template<class Key, class Value = std::conditional_t<std::is_const_v<Key>, Value const, Value>>
    static auto _toValuesPointer(Key* ptr) -> Value* {
        auto aligned_address = (std::bit_cast<uintptr_t>(ptr) + value_align_offset) & value_align_mask;
        return std::bit_cast<Value*>(aligned_address);
    }

    auto keyToValue(Key const* key) -> Value& { return _valueData()[key - m_keyData]; }
    auto keyToValue(Key const* key) const -> Value const& { return _valueData()[key - m_keyData]; }

    [[nodiscard]] auto grownStorage(int growBy) const -> KeyAmendSpan {
        auto cur = m_capacity;
        auto res = (cur << 1) - (cur >> 1) + (cur >> 4); // * 1.563
        if (res < 5) res = 5;
        if (res < m_capacity + growBy) res = m_capacity + growBy;
        auto ptr = OrderedMapArray::allocate(res);
        return KeyAmendSpan{ptr, res};
    }
    void growBy(int by) {
        auto newStorage = grownStorage(by);
        KeyUtils::moveConstruct(newStorage.begin(), moveKeys());
        ValueUtils::moveConstruct(_toValuesPointer(newStorage.end()), moveValues());
        KeyUtils::destruct(_keyAmend());
        ValueUtils::destruct(amendValues());
        OrderedMapArray::deallocate(_amendCapacity());
        m_keyData = newStorage.begin();
        m_capacity = newStorage.count();
    }

    auto add(Key key) -> Value* {
        if (m_count == m_capacity) {
            return addWithGrowing(key);
        }
        return addToCapacity(key);
    }
    auto addToCapacity(Key key) -> Value* {
        auto keyRange = orderedKeys().equalRange(key);
        if (!keyRange.isEmpty()) {
            return nullptr;
        }
        auto keyIt = const_cast<Key*>(keyRange.begin());
        auto index = static_cast<Index>(keyIt - m_keyData);
        auto valueIt = _valueData() + index;
        if (index != m_count) {
            KeyUtils::moveAssignForward(keyIt + 1, moveKeys().skip(index));
            ValueUtils::moveAssignForward(valueIt + 1, moveValues().skip(index));
        }
        KeyUtils::moveAssign(keyIt, KeyMoveSpan{&key, 1});
        m_count++;
        return valueIt;
    }
    auto addWithGrowing(Key key) -> Value* {
        auto keyRange = orderedKeys().equalRange(key);
        if (!keyRange.isEmpty()) {
            return nullptr;
        }
        auto keyIt = const_cast<Key*>(keyRange.begin());
        auto newStorage = grownStorage(1);
        auto nKeyPtr = newStorage.begin();
        auto nValuePtr = _toValuesPointer(newStorage.end());
        auto index = static_cast<Index>(keyIt - m_keyData);
        if (0 != index) {
            KeyUtils::moveConstruct(nKeyPtr, moveKeys().head(index));
            ValueUtils::moveConstruct(nValuePtr, moveValues().head(index));
            nKeyPtr += index;
            nValuePtr += index;
        }
        KeyUtils::moveConstruct(nKeyPtr, KeyMoveSpan{&key, 1});
        // note: value is added outside
        if (index != m_count) {
            KeyUtils::moveConstruct(nKeyPtr + 1, moveKeys().skip(index));
            ValueUtils::moveConstruct(nValuePtr + 1, moveValues().skip(index));
        }
        deallocate(_amendCapacity());
        m_keyData = newStorage.begin();
        m_capacity = newStorage.count();
        m_count++;
        return nValuePtr;
    }

    auto inplaceMerge(KeyConstOrderedSpan newKeys, Value const* newValues) -> Index {
        auto const less = Less{};
        auto totalCount = newKeys.count() + m_count;
        auto const keyPtr = m_keyData;
        auto const valuePtr = _valueData();
        auto const oldBegin = m_keyData - 1;
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
                KeyUtils::copyConstruct(keyPtr + outIndex, KeyConstSpan{newBegin + 1 + newCount, constructCount});
                ValueUtils::copyConstruct(valuePtr + outIndex, ValueConstSpan{newValues + newCount, constructCount});
            }
            if (newCount > 0) {
                outIndex -= newCount;
                KeyUtils::copyAssign(keyPtr + outIndex, KeyConstSpan{newBegin + 1, newCount});
                ValueUtils::copyAssign(valuePtr + outIndex, ValueConstSpan{newValues, newCount});
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
                    KeyUtils::moveConstruct(keyPtr + outIndex, KeyMoveSpan{oldB + 1 + count, constructCount});
                    ValueUtils::moveConstruct(
                        valuePtr + outIndex,
                        ValueMoveSpan{valuePtr + (oldB - oldBegin + count), constructCount});
                }
                if (count > 0) {
                    outIndex -= count;
                    KeyUtils::moveAssignForward(keyPtr + outIndex, KeyMoveSpan{oldB + 1, count});
                    ValueUtils::moveAssignForward(
                        valuePtr + outIndex,
                        ValueMoveSpan{valuePtr + (oldB - oldBegin), count});
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
                    KeyUtils::copyConstruct(keyPtr + outIndex, KeyConstSpan{newB + 1 + count, constructCount});
                    ValueUtils::copyConstruct(
                        valuePtr + outIndex,
                        ValueConstSpan{newValues + (newB - newBegin) + count, constructCount});
                }
                if (count > 0) {
                    outIndex -= count;
                    KeyUtils::copyAssign(keyPtr + outIndex, KeyConstSpan{newB + 1, count});
                    ValueUtils::copyAssign(valuePtr + outIndex, ValueConstSpan{newValues + (newB - newBegin), count});
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
                KeyMoveSpan{keyPtr + offset + equalCount, totalCount - offset});
            ValueUtils::moveAssignReverse(
                valuePtr + offset,
                ValueMoveSpan{valuePtr + offset + equalCount, totalCount - offset});
        }
        return totalCount;
    }
    auto mergeConstructInto(KeyAmendSpan newStorage, KeyConstOrderedSpan newKeys, Value const* newValues) -> Index {
        auto const less = Less{};
        auto const keyPtr = newStorage.begin();
        auto const valuePtr = _toValuesPointer(newStorage.end());
        auto const oldBegin = m_keyData;
        auto const oldEnd = oldBegin + m_count;
        auto const oldValuePtr = _valueData();
        auto const newBegin = newKeys.begin();
        auto const newEnd = newKeys.end();
        auto oldIt = oldBegin;
        auto newIt = newBegin;
        auto totalCount = Index{};
        auto finishNew = [&]() {
            auto newCount = static_cast<Index>(newEnd - newIt);
            KeyUtils::copyConstruct(keyPtr + totalCount, KeyConstSpan{newIt, newCount});
            ValueUtils::copyConstruct(valuePtr + totalCount, ValueConstSpan{newValues + (newIt - newBegin), newCount});
            totalCount += newCount;
        };
        auto finishOld = [&]() {
            auto oldCount = static_cast<Index>(oldEnd - oldIt);
            KeyUtils::moveConstruct(keyPtr + totalCount, KeyMoveSpan{oldIt, oldCount});
            ValueUtils::moveConstruct(valuePtr + totalCount, ValueMoveSpan{oldValuePtr + (oldIt - oldBegin), oldCount});
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
                KeyUtils::copyConstruct(keyPtr + totalCount, KeyConstSpan{newIt, count});
                ValueUtils::copyConstruct(valuePtr + totalCount, ValueConstSpan{newValues + (newIt - newBegin), count});
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
                KeyUtils::moveConstruct(keyPtr + totalCount, KeyMoveSpan{oldIt, count});
                ValueUtils::moveConstruct(
                    valuePtr + totalCount,
                    ValueMoveSpan{oldValuePtr + (oldIt - oldBegin), count});
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
