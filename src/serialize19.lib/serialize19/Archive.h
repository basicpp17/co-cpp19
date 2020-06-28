#pragma once
#include "ArchiveMode.h"

#include <concepts> // convertible_to

namespace serialize19 {

// clang-format off
/// Various kinds of Archives are used for serialisation
/// see:
/// - ReadArchive
/// - SizeArchive
/// - WriteToArchive
template<class A> concept Archive = requires(A& a, int v) {
    { A::mode } -> std::convertible_to<ArchiveMode>;
    { a.withPrimitive(v) };
};
// clang-format on

} // namespace serialize19
