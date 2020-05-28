
#pragma once
#include "Strong.h"
#include "string19/View.ostream.h"

#include <ostream>

namespace strong19 {

template<class S> inline auto strongOstream(std::ostream& out, const S& s) -> std::ostream& {
    return out << strong_name<S> << ": " << s.v;
}

} // namespace strong19

#define STRONG_OSTREAM(NAME)                                                                                           \
    inline auto operator<<(std::ostream& out, const NAME& s)->std::ostream& { return strong19::strongOstream(out, s); }
