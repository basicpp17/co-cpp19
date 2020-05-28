#pragma once
#include "IndexPack.h"
#include "nullptr_to.h"

namespace meta19 {

template<size_t... Vs> inline constexpr auto& index_pack = nullptr_to<IndexPack<Vs...>>;

} // namespace meta19
