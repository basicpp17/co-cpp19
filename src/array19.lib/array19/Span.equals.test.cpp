#include "Span.carray.h"
#include "Span.equals.h"

using namespace array19;

void constexpr_Span_equals_test() {
    constexpr static int a[3] = {1, 2, 3};
    constexpr auto as = spanOfCArray(a);

    constexpr static int b[3] = {1, 2, 3};
    constexpr auto bs = amendSpanOfCArray(b);

    static_assert(as == bs);
}

void constexpr_Span_equals_empty_test() {
    constexpr auto as = Span<int>{};
    constexpr auto bs = Span<int>{};
    static_assert(as == bs);
}

void constexpr_Span_equals_count_differ_test() {
    constexpr static int a[3] = {1, 2, 3};
    constexpr auto as = spanOfCArray(a);

    constexpr auto bs = Span<const int>{};

    static_assert(as != bs);
}

void constexpr_Span_equals_value_differ_test() {
    constexpr static int a[3] = {1, 2, 3};
    constexpr auto as = spanOfCArray(a);

    constexpr static int b[3] = {1, 99, 3};
    constexpr auto bs = amendSpanOfCArray(b);

    static_assert(as != bs);
}
