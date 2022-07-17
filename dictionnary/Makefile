all: rust_dict cpp_dict

rust_dict: dict.rs
	rustc -C prefer-dynamic -C opt-level=3 $^ -o $@

cpp_dict: dict.cpp
	g++-11 -std=c++17 -Wall -Wextra -Wpedantic -O3 $^ -o $@

.PHONY: clean

clean:
	-rm *dict
