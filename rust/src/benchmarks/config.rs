use super::dictionnary::benchmark_dictionnary;
use super::algebra::benchmark_algebra;
use super::vector::{benchmark_vector1, benchmark_vector2, benchmark_vector3};
use super::array::benchmark_array;
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
                ("dictionnary", &benchmark_dictionnary as &dyn Fn() -> ()),
                ("algebra", &benchmark_algebra as &dyn Fn() -> ()),
                ("vector1", &benchmark_vector1 as &dyn Fn() -> ()),
                ("vector2", &benchmark_vector2 as &dyn Fn() -> ()),
                ("vector3", &benchmark_vector3 as &dyn Fn() -> ()),
                ("array", &benchmark_array as &dyn Fn() -> ()),
            ])
        }
    }
}
