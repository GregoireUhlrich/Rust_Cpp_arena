#ifndef BCH_BENCHMARK_H_INCLUDED
#define BCH_BENCHMARK_H_INCLUDED

#include "timer.h"
#include <vector>

namespace bch {

template <class Callable>
Timer::time_t benchmark_ms(Callable &&f)
{
    Timer timer;
    f();
    return timer.get_duration_ms();
}

template <class Callable>
std::vector<Timer::time_t> benchmark_multiple_ms(Callable  &&f,
                                                 std::size_t n_iter)
{
    std::vector<Timer::time_t> benchmark(n_iter);
    for (std::size_t i = 0; i != n_iter; ++i) {
        benchmark[i] = benchmark_ms(std::forward<Callable>(f));
    }
    return benchmark;
}

} // namespace bch

#endif
