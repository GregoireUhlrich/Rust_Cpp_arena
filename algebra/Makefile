all: rust_algebra cpp_algebra

rust_algebra: algebra.rs
	rustc -C prefer-dynamic -C opt-level=3 $^ -o $@

cpp_algebra: algebra.cpp
	g++-11 -std=c++17 -Wall -Wextra -Wpedantic -O3 $^ -o $@

.PHONY: clean

clean:
	-rm *algebra
