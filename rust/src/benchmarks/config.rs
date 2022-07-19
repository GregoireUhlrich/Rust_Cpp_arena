use super::dictionnary::benchmark_dictionnary;
use super::algebra::benchmark_algebra;
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
                ("algebra", &benchmark_algebra as &dyn Fn() -> ())
            ])
        }
    }
}
