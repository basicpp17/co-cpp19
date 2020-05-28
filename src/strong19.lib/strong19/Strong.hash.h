#pragma once
#include "Strong.h"

#include <functional> // std::hash

namespace strong19 {

template<class S> struct hash {
    size_t operator()(const S& s) const { return std::hash<StrongType<S>>{}(s.v); }
};

} // namespace strong19
