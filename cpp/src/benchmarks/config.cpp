#include "config.h"
#include "algebra.h"
#include "algorithm.h"
#include "array.h"
#include "dictionnary.h"
#include "threads.h"
#include "vector.h"

namespace bch {
const std::unordered_map<std::string, std::function<void()>> benchmarks
    = {{"dictionnary", benchmark_dictionnary},
       {"algebra", benchmark_algebra},
       {"vector1", benchmark_vector1},
       {"vector2", benchmark_vector2},
       {"vector3", benchmark_vector3},
       {"array", benchmark_array},
       {"threads", benchmark_threads},
       {"algorithm", benchmark_algorithm}};
} // namespace bch
