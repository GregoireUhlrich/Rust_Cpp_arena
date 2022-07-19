#include "benchmarks/config.h"
#include "tools/benchmark.h"
#include "tools/timer.h"
#include <fstream>
#include <iostream>

std::tuple<std::string, std::string, std::size_t>
parse_args(int argc, char const *argv[])
{
    if (argc < 3) {
        std::cerr << "This program needs 2 arguments:\n";
        std::cerr << " 1. The benchmark name to execute\n";
        std::cerr << " 2. The output file name in results/\n";
        std::cerr << " 3. (optional, default=1) The number of iterations to "
                     "execute\n";
        exit(1);
    }
    std::string benchmark = argv[1];
    std::string result    = argv[2];
    std::size_t n_iter;
    try {
        n_iter = (argc == 3 ? 1 : std::stoul(std::string(argv[3])));
    }
    catch (std::exception const &err) {
        std::cerr << "Invalid argument for the number of iterations \""
                  << argv[3] << "\".\n";
        std::cerr << err.what() << std::endl;
        exit(1);
    }
    return {benchmark, result, n_iter};
}

std::function<void()> get_benchmarked_function(std::string const &name)
{
    auto pos = bch::benchmarks.find(name);
    if (pos == bch::benchmarks.end()) {
        std::cerr << "Benchmark \"" << name << "\" does not exist.\n";
        std::cerr << "Available benchmarks:\n";
        for (const auto &[bname, _] : bch::benchmarks) {
            std::cerr << " -> \"" << bname << "\"\n";
        }
        exit(2);
    }
    return pos->second;
}

std::vector<bch::Timer::time_t>
get_benchmark(std::function<void()> const &benchmark_func, std::size_t n_iter)
{
    try {
        return bch::benchmark_multiple_ms(benchmark_func, n_iter);
    }
    catch (std::exception const &err) {
        std::cerr << "Aborting benchmarking because of uncaught "
                     "exception:\n";
        std::cerr << err.what() << std::endl;
        exit(4);
    }
}

void write_to_file(std::vector<bch::Timer::time_t> const &benchmark,
                   std::string const                     &filename)
{
    std::ofstream res(RESULTS_DIR "/" + filename);
    if (!res) {
        std::cerr << "File \"" RESULTS_DIR "/" << filename
                  << "\" is invalid.\n";
        exit(3);
    }
    for (const auto &time : benchmark) {
        res << time << '\n';
    }
    res.close();
}

int main(int argc, char const *argv[])
{
    auto [benchmark_name, res_name, n_iter] = parse_args(argc, argv);
    auto benchmark_func = get_benchmarked_function(benchmark_name);
    std::vector<bch::Timer::time_t> benchmark
        = get_benchmark(benchmark_func, n_iter);
    write_to_file(benchmark, res_name);
}
