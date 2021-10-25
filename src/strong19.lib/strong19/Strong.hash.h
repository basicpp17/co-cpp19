#pragma once
#include "Strong.h"

#include <stddef.h> // size_t
#include <functional> // std::hash

namespace strong19 {

template<class S> struct hashFor {
    size_t operator()(const S& s) const { return std::hash<StrongValueType<S>>{}(s.v); }
};

} // namespace strong19
