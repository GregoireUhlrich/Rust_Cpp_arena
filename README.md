# Rust_Cpp_arena

Playground to learn Rust and compare several features with C++. The corresponding source code are separated in two projects, `cpp` and `rust`. They implement the same benchmark programs in `src/benchmarks` and the main program in `main.rs/main.cpp`.

## Run the benchmarks

``` bash
    python benchmark.py
```
This will launch the pre-defined benchmarks and plot the results.

## Benchmark list 

 - *dictionnary*: Build a dictionnary containing ~40000 words from a text file.
 The dictionnary is a specific tree structure using hash maps to store words
 and provide a fast access in `O(n)` with `n` the number of letters in the word.
 - *algebra*: An extremely simple symbolic manipulation program to create and
 compare expressions, and replace sub-expressions. Highly dynamical program.
 - *error*: Error handling. This example is quite unfair for `C++` because 
 we compare exception handling in `C++` with the much simpler `Rust` system 
 (monade).
 - *option*: As the error handling but this time using optional objects both
 in `C++` and `Rust`. For some reason `C++` can optimize out most of the 
 program.
 - *vector1*: Allocate a vector of size `500 000` and copy it in a second 
 vector.
 - *vector2*: Allocate an empty vector and push elements up to a size 
 of `500 000`.
 - *vector3*: Same as *vector2* but reserving the relevant memory space 
 beforehand.
 - *array*: Iterating through a `1000x1000` static array.
 - *threads*: Increment an atomic counter up to `10 000 000` using `10`
 threads.
 - *memoization*: Memoization program for minimum path length between
 two coordinates in a `1000x1000` square (use hash maps).
 - *algorithm*: Iteration (with mutability) over a range of integers
 (size `100 000`). Using iterators in `Rust` and ranges in `C++`.

### Example of benchmark results
![](assets/example.png)

## Add new benchmarks

You should implement the benchmark, including the main benchmark function (should take no parameter and return nothing) in `src/benchmarks/` both in the `cpp` and `rust` projects.

Then, modify the `src/tools/config.rs` and `src/tools/config.cpp` files to add the benchmark function in the `rust` and `cpp` projects respectively, by choosing one particular name. This gives for the `rust` configuration:
``` rust
    benchmarks: HashMap::from([
        ...,
        ("benchmark_name", &benchmark_rust_func as &BenchmarkFunc),
    ])
```
And the `cpp` counter-part:
``` cpp
    std::unordered_map<std::string, std::function<void()>> benchmarks = {
        ...,
        {"benchmark_name", benchmark_cpp_func},
    };
```

Finally, add the benchmark in the `BENCHMARKS` variable of the `benchmark.py` file, completing the list a tuple:
``` python
    BENCHMARKS = [
        ...,
        "benchmark_name",
    ]
```
