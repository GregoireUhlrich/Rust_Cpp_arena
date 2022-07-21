#ifndef BCH_DYNAMIC_H_INCLUDED
#define BCH_DYNAMIC_H_INCLUDED

#include <cstdlib>
#include <ctime>

inline int dynamic_int(int n = 10)
{
    static bool init = false;
    if (!init) {
        srand(time(0));
        init = true;
    }
    return rand() % n;
}

#endif
