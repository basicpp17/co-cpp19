#pragma once
#include "Strong.h"
#include "meta19/TypePackTemplate.contains.h"

namespace strong19 {

using meta19::type_pack_contains;

template<class S, class... Tags> constexpr bool has_strong_tags = (type_pack_contains<StrongTags<S>, Tags> && ...);

} // namespace strong19
