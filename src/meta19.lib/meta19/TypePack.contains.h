#pragma once
#include "Type.h"
#include "TypePack.h"
#include "isSame.h"

namespace meta19 {

template<class... Ts, class T> constexpr auto typePackContains(TypePack<Ts...>* = {}, Type<T>* = {}) {
    return (isSame(&type<Ts>, &type<T>) || ...);
}

template<class Tp, class T> constexpr bool type_pack_contains = typePackContains(nullptr_to<Tp>, type<T>);

} // namespace meta19
