#include <meta19/IndexPack.h>
#include <meta19/index_pack.back.h>

using namespace meta19;

void pack_back_test() { //
    static_assert(pack_back<12, 13> == 13);
}

void index_pack_back_test() {
    static_assert(index_pack_back<IndexPack<12, 13>> == 13);
    static_assert(index_pack_back<IndexPack<>> == nullptr);
}

int main() { 
    pack_back_test();
    index_pack_back_test();
    return 0;
}