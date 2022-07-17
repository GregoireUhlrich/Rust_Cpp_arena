import os
import sys
import subprocess
from time import perf_counter_ns


def bash_iteration(cmd: str, n_iter: int) -> str:
    return f'for i in `seq 1 {n_iter}`; do {cmd}; done;'


def _benchmark(out, dir: str, file: str, compile_cmd: str, n_iter: int):
    compile_cmd = (
        f"cd {dir}; rm a.out &>/dev/null; "
        + bash_iteration(compile_cmd, n_iter)
    )
    start_c = perf_counter_ns()
    subprocess.run(compile_cmd, shell=True, executable='/bin/bash')
    end_c = perf_counter_ns()

    size = os.path.getsize(os.path.join(dir, 'a.out'))

    run_cmd = "./a.out"
    run_cmd = f"cd {dir}; " + bash_iteration(run_cmd, n_iter)

    start_r = perf_counter_ns()
    os.system(run_cmd)
    end_r = perf_counter_ns()

    out.write(f'{file.ljust(15)}\t{(end_c - start_c)*1e-6/n_iter}\t'
              f'{size/1000.}\t{(end_r - start_r)*1e-6/n_iter}\n')


def benchmark(dir: str, n_iter: int):
    files = [file
             for file in os.listdir(dir)
             if os.path.isfile(os.path.join(dir, file))]
    cpp = [file
           for file in files
           if file.find(".cpp") != -1]
    rust = [file
            for file in files
            if file.find(".rs") != -1]

    out = open(os.path.join(dir, "benchmark.txt"), "w")
    out.write(f'{n_iter} Iterations:\tBuild (ms)'
              '\tSize (kB)\tRun (ms)\n')
    for cfile in cpp:
        print(f'C++ benchmark for {cfile}')
        compile_cmd = (
            f'g++ -std=c++17 -Wall -Wextra -Wpedantic {cfile} -o a.out'
        )
        _benchmark(out, dir, cfile, compile_cmd, n_iter)

    for rfile in rust:
        print(f'Rust benchmark for {rfile}')
        compile_cmd = (
            f'rustc -C prefer-dynamic {rfile} -o a.out'
        )
        _benchmark(out, dir, rfile, compile_cmd, n_iter)


def main():
    if len(sys.argv) < 2:
        print("This program needs one argument:"
              " the directory name to benchmark.")
        exit()
    dir = sys.argv[1]
    try:
        benchmark(dir, max(1, int(sys.argv[2])) if len(sys.argv) > 2 else 1)
    finally:
        exe = os.path.join(dir, 'a.out')
        if os.path.exists(exe):
            os.remove(exe)


if __name__ == '__main__':
    main()