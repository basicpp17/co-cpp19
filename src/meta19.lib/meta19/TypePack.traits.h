#pragma once
#include <type_traits>

namespace meta19 {

template<class... Ts>
constexpr bool all_no_throw_copy_contructible = (true && ... && std::is_nothrow_copy_constructible_v<Ts>);

template<class... Ts>
constexpr bool all_no_throw_copy_assignable = (true && ... && std::is_nothrow_copy_assignable_v<Ts>);

} // namespace meta19
