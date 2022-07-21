#include "algorithm.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace bch {

void benchmark_algorithm()
{
    constexpr static std::size_t N = 100000;
    std::vector<int>             elements(N);
    auto                         generator = [n = 0](int) mutable {
        const int res = n;
        n += 3;
        return res;
    };
    auto      filter = [](int val) { return std::abs(val) > 10; };
    auto      invert = [](int x) { return -x; };
    const int max    = std::ranges::max(
        elements | std::views::transform(generator)
        | std::views::filter(filter) | std::views::transform(invert));
    assert(max == -12);
}

} // namespace bch
