
#include <array19/AllocatedArrayOf.h>
#include <array19/SliceOf.carray.h>
#include <array19/SliceOf.equals.h>
#include <array19/SliceOf.h>
#include <iostream>


using namespace array19;

int main() {
    auto v = AllocatedArrayOf{1, 2, 3};
    auto t = sliceOfCArray({1, 2, 3}) ==  SliceOf{v};
    std::cout << "Test " << std::boolalpha << t;
    return 0;
}