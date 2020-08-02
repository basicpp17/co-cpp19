#pragma once
#include "SortedSlice.h"
#include "array19/DynamicArrayOf.h"

namespace lookup19 {

using array19::DynamicArrayOf;
using array19::SliceOf;

template<class T, class Less> struct SortedArray {
    using Storage = DynamicArrayOf<T>;
    using Element = T;
    using Size = size_t;
    using Index = size_t;

    using Slice = SortedSlice<Element, Less>;
    using ConstSlice = SortedSlice<const Element, Less>;
    using UnorderedSlice = SliceOf<const Element>;

    [[nodiscard]] auto count() const -> Size { return m_storage.count(); }
    [[nodiscard]] auto totalCapacity() const -> Size { return m_storage.totalCapacity(); }
    [[nodiscard]] auto unusedCapacity() const -> Size { return m_storage.unusedCapacity(); }

    [[nodiscard]] auto begin() const& noexcept -> const Element* { return m_storage.begin(); }
    [[nodiscard]] auto end() const& noexcept -> const Element* { return m_storage.end(); }
    [[nodiscard]] operator ConstSlice() const noexcept { return ConstSlice{begin(), count()}; }

    [[nodiscard]] auto amendBegin() & noexcept -> Element* { return m_storage.amendBegin(); }
    [[nodiscard]] auto amendEnd() & noexcept -> Element* { return m_storage.amendEnd(); }
    [[nodiscard]] auto amend() -> Slice { return Slice{amendBegin(), count()}; }

    void insert(UnorderedSlice elems) {
        m_storage.ensureUnusedCapacity(elems.count());
        for (const auto& elem : elems) {
            auto it = this->lowerBound(elem);
            m_storage.splice(it, 0, UnorderedSlice{&elem, 1});
        }
    }

    void remove(ConstSlice slice) { m_storage.splice(const_cast<T*>(slice.begin()), slice.count(), {}); }

private:
    DynamicArrayOf<T> m_storage;
};

} // namespace lookup19
