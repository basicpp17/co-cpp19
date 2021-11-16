#include <lookup19/OrderedSetOf.h>

using namespace lookup19;

int main(){

    auto v = OrderedSetOf<int>{};

    v.ensureUnusedCapacity(1);

    v.insert(12);
    v.insert(23);
    v.insert(42);
    v.insert(12);
    v.remove(OrderedSliceOf{v}.lowerBound(12));

    return 0;
}