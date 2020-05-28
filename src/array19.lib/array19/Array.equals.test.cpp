#include "Array.equals.h"
#include "Array.h"

using namespace array19;

void constexpr_Array_equals_test() {
    constexpr static auto a = Array{1, 2, 3};
    constexpr static auto b = Array{1, 2, 3};
    static_assert(a == b);
}

void constexpr_Array_equals_count_differ_test() {
    constexpr static auto a = Array{1, 2, 3};
    constexpr static auto b = Array{1, 2, 3, 4};
    static_assert(a != b);
}

void constexpr_Array_equals_value_differ_test() {
    constexpr static auto a = Array{1, 2, 3};
    constexpr static auto b = Array{1, 99, 3};
    static_assert(a != b);
}
