#include "threads.h"
#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace bch {
void benchmark_threads()
{
    constexpr unsigned long long n_threads = 10;
    constexpr unsigned long long n_iter    = 1'000'000;
    unsigned long long           counter   = 0;
    std::vector<std::thread>     threads;
    threads.reserve(n_threads);
    std::mutex mx;
    for (std::size_t i = 0; i != n_threads; ++i) {
        threads.emplace_back([n_iter, &counter, &mx]() {
            for (std::size_t i = 0; i != n_iter; ++i) {
                std::lock_guard<std::mutex> guard{mx};
                ++counter;
            }
        });
    }
    for (std::size_t i = 0; i != n_threads; ++i) {
        threads[i].join();
    }
    assert(counter == n_threads * n_iter);
}
} // namespace bch
