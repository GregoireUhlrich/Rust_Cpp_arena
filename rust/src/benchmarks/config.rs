use super::dictionnary::benchmark_dictionnary;
use super::algebra::benchmark_algebra;
use super::vector::{benchmark_vector1, benchmark_vector2, benchmark_vector3};
use super::array::benchmark_array;
use super::threads::benchmark_threads;
use super::memoization::benchmark_memoization;
use super::error::{benchmark_error, benchmark_option};
use super::algorithm::benchmark_algorithm;
use std::collections::HashMap;

type BenchmarkFunc = dyn Fn() -> ();

pub struct Config {
    pub benchmarks: HashMap<&'static str, &'static BenchmarkFunc>
    // test: Vec<&'static str>
}

impl Default for Config {
    fn default() -> Config {
        // Config { test: Vec::from(["w", "r"]) }
        Config {
            benchmarks: HashMap::from([
                ("dictionnary", &benchmark_dictionnary as &BenchmarkFunc),
                ("algebra", &benchmark_algebra as &BenchmarkFunc),
                ("vector1", &benchmark_vector1 as &BenchmarkFunc),
                ("vector2", &benchmark_vector2 as &BenchmarkFunc),
                ("vector3", &benchmark_vector3 as &BenchmarkFunc),
                ("array", &benchmark_array as &BenchmarkFunc),
                ("threads", &benchmark_threads as &BenchmarkFunc),
                ("memoization", &benchmark_memoization as &BenchmarkFunc),
                ("error", &benchmark_error as &BenchmarkFunc),
                ("option", &benchmark_option as &BenchmarkFunc),
                ("algorithm", &benchmark_algorithm as &BenchmarkFunc),
            ])
        }
    }
}
