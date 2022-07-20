use std::env;
mod tools;
use tools::benchmark::benchmark_multiple;
mod benchmarks;
use benchmarks::config::Config;
use tools::paths::get_results_path;
use std::fs::File;
use std::io::{BufWriter, Write};

fn parse_arguments() -> (String, String, usize)
{
    let args : Vec<String> = env::args().collect();
    if args.len() < 3 {
        panic!(
            r#"
            This program needs 2 arguments:
              1. The name of the benchmark function to call
              2. The name of the result output file in results
              3. (optional, default=1) The number of iterations for the benchmark"#)
    }
    let benchmark = args[1].clone();
    let res = args[2].clone();
    let n_iter = if args.len() > 3 { args[3].parse::<usize>().unwrap() } else { 1 };
    (benchmark, res, n_iter)
}

fn get_benchmark_func<'a>(config: &'a Config, name: &'a str) -> &'a dyn Fn()->()
{
    let res = config.benchmarks.get(name);
    match res {
        Some(f) => *f,
        None => panic!("Benchmark of name \"{}\" not known.", name)
    }
}

fn write_to_file(res: &Vec<f32>, filename: &str) -> ()
{
    let mut path = get_results_path();
    path.push(filename);
    let file = File::create(path).unwrap();
    let mut buf_writer = BufWriter::new(file);
    for value in res.iter() {
        buf_writer.write_fmt(format_args!("{}\n", value)).unwrap();
    }
}

fn main() {
    let config = Config::default();
    let (benchmark_name, res_name, n_iter) = parse_arguments();
    let benchmark_func = get_benchmark_func(&config, &benchmark_name);
    let res :Vec<f32> = benchmark_multiple(&benchmark_func, n_iter);
    write_to_file(&res, &res_name);
}
