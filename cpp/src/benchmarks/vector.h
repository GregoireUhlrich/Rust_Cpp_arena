#ifndef BCH_VECTOR_H_INCLUDED
#define BCH_VECTOR_H_INCLUDED

#include <string>
#include <vector>

namespace bch {

struct Object {
    int                      id      = 0;
    std::string              version = "Some version";
    std::vector<std::string> partners
        = {"A", "BB", "CCC", "DDDD", "ZZZZZZZZZZZZZZZZZZZZZZZZZ"};
};

void benchmark_vector1();
void benchmark_vector2();
void benchmark_vector3();
} // namespace bch

#endif
