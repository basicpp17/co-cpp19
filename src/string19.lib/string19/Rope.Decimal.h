#include "ADL.h"
#include "meta19/nullptr_to.h"

#include <stddef.h> // size_t

namespace string19 {

using meta19::nullptr_to;

template<size_t N> struct Decimal;
template<size_t N> constexpr auto& decimal = nullptr_to<Decimal<N>>;

template<size_t N> constexpr auto ropeCount(ADL*, Decimal<N>*) -> size_t {
    auto c = 1u;
    for (auto n = 10u; n <= N; n *= 10) c++;
    return c;
}

template<size_t N> constexpr void ropeAppend(ADL*, char*& data, Decimal<N>*) {
    auto c = ropeCount(adl, decimal<N>);
    auto n = N;
    for (int o = c - 1; o >= 0; o--, n /= 10) data[o] = n % 10 + '0';
    data += c;
}

} // namespace string19
