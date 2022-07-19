#include <functional>
#include <string>
#include <unordered_map>

namespace bch {

extern const std::unordered_map<std::string, std::function<void()>> benchmarks;
} // namespace bch
