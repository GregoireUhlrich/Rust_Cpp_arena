all: hello

hello: hello.rs
	rustc $^ -o $@

.PHONY: clean

clean:
	-rm hello
