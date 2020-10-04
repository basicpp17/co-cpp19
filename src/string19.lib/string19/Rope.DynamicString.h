#pragma once
#include "ADL.h"
#include "DynamicString.h"

namespace string19 {

inline auto ropeLengthOf(ADL*, const DynamicString& str) -> size_t { return str.length(); }

inline void ropeAppend(ADL*, char*& it, const DynamicString& str) { it = std::copy(str.begin(), str.end(), it); }

inline bool ropeCombine(ADL*, DynamicString& str, char chr) {
    if (str.capacity() > str.length()) {
        str += chr;
        return true;
    }
    return false;
}

inline bool ropeCombine(ADL*, DynamicString& str, const DynamicString& ostr) {
    if (str.capacity() > str.length() + ostr.length()) {
        str += ostr;
        return true;
    }
    return false;
}

} // namespace string19
