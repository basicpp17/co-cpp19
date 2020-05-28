#pragma once

namespace meta19 {

constexpr bool isSame(const void* a, const void* b) { return a == b; }

template<class T, class U> constexpr bool isSame(T*&, U*&) {
    static_assert((sizeof(T), sizeof(U), false), "HINT: use &type<T>!");
    return false;
}

} // namespace meta19
