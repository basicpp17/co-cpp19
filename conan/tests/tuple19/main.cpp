#include <tuple19/Tuple.h>
using namespace tuple19;

int main() {
    using T = Tuple<char, int, double>;

    static_assert(T::has_type<char>);
    static_assert(T::has_type<int>);
    static_assert(T::has_type<double>);
    static_assert(!T::has_type<float>);
    static_assert(!T::has_type<bool>);
    static_assert(!T::has_type<unsigned int>);

    static_assert(T::index_of<char> == 0);
    static_assert(T::index_of<int> == 1);
    static_assert(T::index_of<double> == 2);

    using meta19::_index;
    static_assert(std::is_same_v<const char&, decltype(std::declval<T>().at(_index<0>))>);
    return 0;
}