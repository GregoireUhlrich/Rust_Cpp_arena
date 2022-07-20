#!/bin/python3

import os
import subprocess
import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass


N_ITER = 10


BENCHMARKS = [
    ("dictionnary", "dict.txt", N_ITER),
    ("algebra", "algebra.txt", N_ITER),
    ("vector1", "vec1.txt", N_ITER),
    ("vector2", "vec2.txt", N_ITER),
    ("vector3", "vec3.txt", N_ITER),
    ("array", "array.txt", N_ITER),
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
        "cd rust; rm -r target; cargo rustc --release -- -Cprefer-dynamic"
    )
    run_cmd(cmd)


def cpp_build():
    cmd = (
        "cd cpp; mkdir -p build; cd build; rm -rf * .*; cmake .. && make"
    )
    run_cmd(cmd)


def rust_benchmark(benchmark, resfile, n_iter):
    cmd = (
        f"cd rust; ./target/release/rust {benchmark} {resfile} {n_iter}"
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
        print(item("[rust running]"))
        rust_data = rust_benchmark(benchmark, resfile, n_iter)
        print(item("[c++ running]"))
        cpp_data = cpp_benchmark(benchmark, resfile, n_iter)
        res.append({
            "name": benchmark,
            "cpp": Benchmark(**cpp_data),
            "rust": Benchmark(**rust_data)
        })
    print(msg("End of benchmarks"))
    return res


def plot_benchmarks(benchmarks):
    x = [
        el
        for data in benchmarks
        for el in [data["cpp"].runtime, data["rust"].runtime]
    ]
    s = [
        el
        for data in benchmarks
        for el in [data["cpp"].size, data["rust"].size]
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
    plot_sizes = False
    fig, axes = plt.subplots(1 + plot_sizes, n_plots,
                             figsize=(3*n_plots, 3.5*(1+plot_sizes)))
    for i in range(n_plots):
        ax = axes[0][i] if plot_sizes else axes[i]
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
        if i == 0:
            ax.set_ylabel(r'Runtime $[\mathrm{ms}]$', fontsize=label_size)
        if not plot_sizes:
            ax.set_xticks([-1, 1])
            ax.set_xticklabels(["cpp", "rust"], fontsize=label_size)
            ax.tick_params(axis='x', which='major', labelsize=label_size)
            ax.tick_params(axis='x', which='minor', labelsize=label_size)
        else:
            ax2 = axes[1][i]
            ax2.bar(-1, height=s[2*i]/1024, width=0.75,
                    color=cpp_color, label='C++')
            ax2.bar(+1, height=s[2*i+1]/1024, width=0.75,
                    color=rust_color, label='Rust')
            ax.set_xticks([-1, 1])
            ax.set_xticklabels(["cpp", "rust"], fontsize=label_size)
            ax.tick_params(axis='x', which='major', labelsize=label_size)
            ax.tick_params(axis='x', which='minor', labelsize=label_size)
            ax2.tick_params(axis='y', which='major', labelsize=ticks_size)
            ax2.tick_params(axis='y', which='minor', labelsize=ticks_size)
            if i == 0:
                ax2.set_ylabel(
                    r'Executable size $[\mathrm{kB}]$',
                    fontsize=label_size)

    plt.show()


def main():
    benchmarks = run_benchmarks()
    plot_benchmarks(benchmarks)


if __name__ == '__main__':
    main()
