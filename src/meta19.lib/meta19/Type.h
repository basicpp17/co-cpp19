#pragma once
#include "nullptr_to.h"

namespace meta19 {

template<class> struct Type;
template<class T> struct Type<Type<T>> { static_assert((sizeof(T), false), "nested Type!"); };

template<class T> inline constexpr auto& type = nullptr_to<Type<T>>;

} // namespace meta19
