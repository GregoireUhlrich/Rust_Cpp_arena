all: rust_morpion cpp_morpion

rust_morpion: morpion.rs
	rustc -C prefer-dynamic $^ -o $@

cpp_morpion: morpion.cpp
	g++ -std=c++17 $^ -o $@

.PHONY: clean

clean:
	-rm *morpion
