#include "lru.hpp"
#include <cassert>

using namespace lru;

int main()
{
    lru_cache<int, int> lc(2);
    lc.put(1,1);
    lc.put(2,2);
    lc.put(1, 5);
    lc.put(3, 3);
    assert(lc.get(1) == 5);
    assert(lc.get(2) == 2);
}
