#ifndef BCH_TIMER_H_INCLUDED
#define BCH_TIMER_H_INCLUDED

#include <chrono>
#include <type_traits>

namespace bch {

class Timer {
  public:
    using clock_t      = std::chrono::high_resolution_clock;
    using resolution_t = std::chrono::milliseconds;
    using time_point_t = clock_t::time_point;
    using duration_t   = clock_t::duration;

    using time_t = decltype(std::declval<duration_t &>().count());

    Timer()
    {
        start();
    }

    void start()
    {
        _start = this->now();
    }

    time_t get_duration_ms() const
    {
        auto now = this->now();
        return std::chrono::duration_cast<resolution_t>(now - _start).count();
    }

  protected:
    time_point_t now() const
    {
        return clock_t::now();
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};
} // namespace bch

#endif
