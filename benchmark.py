#!/bin/python3

import os
import subprocess
import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass


N_ITER = 10


BENCHMARKS_NAMES = [
    "dictionnary",
    "algebra",
    "error",
    "option",
    "vector1",
    "vector2",
    "vector3",
    "array",
    "threads",
    "memoization",
    "algorithm",
]


BENCHMARKS = [
    (bench, f"{bench}.txt", N_ITER)
    for bench in BENCHMARKS_NAMES
]


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def msg(str):
    return f"{bcolors.OKBLUE}{bcolors.BOLD}{str}{bcolors.ENDC}"


def resmsg(str):
    return f"    --> {bcolors.OKCYAN}{str}{bcolors.ENDC}\n"


def item(str):
    return f"  {bcolors.OKGREEN}- {str}{bcolors.ENDC}"


@dataclass
class Benchmark:
    size: int
    runtime: np.ndarray


def run_cmd(cmd):
    subprocess.run(cmd, shell=True, executable='/bin/bash')


def rust_build():
    cmd = (
        "cd rust && rm -r target; cargo rustc --release -- -Cprefer-dynamic"
    )
    run_cmd(cmd)


def cpp_build():
    cmd = (
        "cd cpp &&mkdir -p build && cd build && rm -rf * .*; cmake .. && make"
    )
    run_cmd(cmd)


def rust_benchmark(benchmark, resfile, n_iter):
    cmd = (
        f"cd rust && ./target/release/rust {benchmark} {resfile} {n_iter}"
    )
    run_cmd(cmd)
    return {
        "size": os.path.getsize("rust/target/release/rust"),
        "runtime": np.loadtxt(f"rust/results/{resfile}")
    }


def cpp_benchmark(benchmark, resfile, n_iter):
    cmd = (
        f"./cpp/build/main {benchmark} {resfile} {n_iter}"
    )
    run_cmd(cmd)
    return {
        "size": os.path.getsize("cpp/build/main"),
        "runtime": np.loadtxt(f"cpp/results/{resfile}")
    }


def run_benchmarks():
    res: list[Benchmark] = []
    print(msg("Building rust library"))
    rust_build()
    print(msg("Building c++ library"))
    cpp_build()
    print(msg(f"Benchmark of {len(BENCHMARKS)} programs:"))
    for bench in BENCHMARKS:
        print(item(str(bench)))
    for benchmark, resfile, n_iter in BENCHMARKS:
        print(msg(f"Running benchmark \"{benchmark}\" ..."))
        print(item("[Rust running]"))
        rust_data = rust_benchmark(benchmark, resfile, n_iter)
        print(item("[C++ running]"))
        cpp_data = cpp_benchmark(benchmark, resfile, n_iter)
        res.append({
            "name": benchmark,
            "cpp": Benchmark(**cpp_data),
            "rust": Benchmark(**rust_data)
        })
        ratio = np.mean(rust_data["runtime"]) / np.mean(cpp_data["runtime"])
        winner = 'C++' if ratio > 1 else 'Rust'
        loser = 'C++' if ratio <= 1 else 'Rust'
        ratio = ratio if ratio > 1 else 1 / ratio
        print(resmsg(f"{winner} wins: {ratio:.2f} times faster than {loser}!"))

    print(msg("End of benchmarks"))
    return res


def plot_benchmarks(benchmarks):
    x = [
        el
        for data in benchmarks
        for el in [data["cpp"].runtime, data["rust"].runtime]
    ]
    labels = [
        data["name"]
        for data in benchmarks
    ]
    cpp_color = "steelblue"
    rust_color = "forestgreen"
    median_color = "red"
    title_size = 15
    label_size = 13
    ticks_size = 11
    n_plots = int(len(x) / 2)
    row_size = 4
    col_size = int(np.ceil(n_plots/row_size))
    fig, axes = plt.subplots(col_size, row_size,
                             figsize=(3.5*row_size, 2.5*col_size))
    for i in range(n_plots):
        ax = axes[int(i / row_size)][i % row_size]
        ax.set_title(r'$\mathrm{' + labels[i] + '}$', fontsize=title_size)
        ax.boxplot(
            [x[2*i]], positions=[-1],
            patch_artist=True,
            boxprops=dict(facecolor=cpp_color, color=cpp_color),
            medianprops=dict(color=median_color),
            widths=[0.75]
            )
        ax.boxplot(
            [x[2*i+1]], positions=[1],
            patch_artist=True,
            boxprops=dict(facecolor=rust_color, color=rust_color),
            medianprops=dict(color=median_color),
            widths=[0.75]
            )

        ax.set_xticks([])
        ax.tick_params(axis='y', which='major', labelsize=ticks_size)
        ax.tick_params(axis='y', which='minor', labelsize=ticks_size)
        if i % row_size == 0:
            ax.set_ylabel(r'Runtime $[\mathrm{ms}]$', fontsize=label_size)
        if (
                (int(i / row_size) == col_size - 1)
                or (int(i / row_size) == col_size - 2
                    and i + row_size >= n_plots)
           ):
            ax.set_xticks([-1, 1])
            ax.set_xticklabels(["cpp", "rust"], fontsize=label_size)
            ax.tick_params(axis='x', which='major', labelsize=label_size)
            ax.tick_params(axis='x', which='minor', labelsize=label_size)
    for j in range(i+1, row_size*col_size):
        axes[int(j / row_size)][j % row_size].set_axis_off()

    plt.show()


def main():
    benchmarks = run_benchmarks()
    plot_benchmarks(benchmarks)


if __name__ == '__main__':
    main()
