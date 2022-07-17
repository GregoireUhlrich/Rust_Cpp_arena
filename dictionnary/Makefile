all: rust_dict cpp_dict

rust_dict: dict.rs
	rustc -C prefer-dynamic $^ -o $@

cpp_dict: dict.cpp
	g++-11 -std=c++17 -Wall -Wextra -Wpedantic $^ -o $@

.PHONY: clean

clean:
	-rm *dict
