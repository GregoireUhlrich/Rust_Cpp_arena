#include "vector.h"
#include <cassert>

namespace bch {
void benchmark_vector1()
{
    std::vector<Object> vec1(500000, Object{});
    auto                vec2 = vec1;
    assert(vec1.size() == vec2.size());
}
void benchmark_vector2()
{
    std::vector<Object> vec1;
    for (int i = 0; i != 500000; ++i) {
        vec1.push_back(Object{});
    }
    auto vec2 = vec1;
    assert(vec1.size() == vec2.size());
}
void benchmark_vector3()
{
    std::vector<Object> vec1;
    vec1.reserve(500000);
    for (int i = 0; i != 500000; ++i) {
        vec1.emplace_back();
    }
    auto vec2 = vec1;
    assert(vec1.size() == vec2.size());
}

} // namespace bch
