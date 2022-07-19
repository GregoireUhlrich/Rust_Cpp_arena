#include "config.h"
#include "algebra.h"
#include "dictionnary.h"

namespace bch {
const std::unordered_map<std::string, std::function<void()>> benchmarks = {
    {"dictionnary", benchmark_dictionnary}, {{"algebra"}, benchmark_algebra}};
} // namespace bch
