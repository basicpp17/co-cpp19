#pragma once
#include "ArchiveMode.h"

#include <type_traits> // is_convertible_v

namespace serialize19 {

// clang-format off
// TODO: replace std::convertible_to on clang11
template<class From, class To>
concept convertible_to = std::is_convertible_v<From, To>;

/// Various kinds of Archives are used for serialisation
/// see:
/// - ReadArchive
/// - SizeArchive
/// - WriteToArchive
template<class A> concept Archive = requires(A& a, int v) {
    { A::mode } -> convertible_to<ArchiveMode>;
    { a.withPrimitive(v) };
};
// clang-format on

} // namespace serialize19
