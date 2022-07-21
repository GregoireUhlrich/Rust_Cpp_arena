#include "array.h"
#include "tools/dynamic.h"
#include <array>
#include <cassert>

namespace bch {
void benchmark_array()
{
    constexpr auto N = 1'000ULL;

    std::array<std::array<unsigned long long, N>, N> arr;
    for (std::size_t i = 0; i != N; ++i) {
        arr[i].fill(0);
    }
    int rand = dynamic_int();
    for (std::size_t i = 0; i != N; ++i) {
        for (std::size_t j = 0; j != N; ++j) {
            if (i != rand) {
                arr[i][j] = i + j;
            }
        }
    }
    unsigned long long sum = 0;
    for (std::size_t i = 0; i != N; ++i) {
        for (std::size_t j = 0; j != N; ++j) {
            sum += arr[i][j];
        }
    }
    assert(sum == N * N * (N - 1) - rand * N - (N * (N - 1)) / 2);
}
} // namespace bch
