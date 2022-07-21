#include "error.h"
#include "tools/dynamic.h"
#include "tools/error.h"
#include <cassert>
#include <optional>

namespace bch {

static unsigned int may_throw(int value, int threshold)
{
    if (value < threshold) {
        throw Error("Oups you gave a negative number!");
    }
    return value;
}
static std::optional<unsigned int> may_fail(int value, int threshold)
{
    if (value < threshold) {
        return std::nullopt;
    }
    return value;
}

void benchmark_error()
{
    constexpr static int N         = 10'000;
    unsigned int         sum       = 0;
    const static int     threshold = dynamic_int();
    for (int i = -N; i != N; ++i) {
        try {
            sum += may_throw(i, threshold);
        }
        catch (Error const &err) {
        }
    }
    assert(sum == 5000 * 9'999 - (threshold * (threshold - 1)) / 2);
}

void benchmark_option()
{
    constexpr static int N         = 10'000;
    unsigned int         sum       = 0;
    const static int     threshold = dynamic_int();
    for (int i = -N; i != N; ++i) {
        sum += may_fail(i, threshold).value_or(0);
    }
    assert(sum == 5000 * 9'999 - (threshold * (threshold - 1)) / 2);
}

} // namespace bch
