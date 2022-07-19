import os
import subprocess
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from dataclasses import dataclass


N_ITER = 10


BENCHMARKS = [
    ("dictionnary", "dict.txt", N_ITER),
    ("algebra", "algebra.txt", N_ITER)
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
        "cd rust; rm -r target; cargo build -r"
    )
    run_cmd(cmd)


def cpp_build():
    cmd = (
        "cd cpp/build; rm -rf * .*; cmake .. && make"
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
    tick_labels = [
        data["name"]
        for data in benchmarks
    ]
    delta = 0.1
    pos = np.arange(len(tick_labels))
    eff_pos = [
        el
        for p in pos
        for el in [p-delta, p+delta]
    ]
    cpp_color = "steelblue"
    rust_color = "forestgreen"
    median_color = "red"
    fig, ax = plt.subplots()
    plt.boxplot(
        x[::2], positions=eff_pos[::2],
        patch_artist=True,
        boxprops=dict(facecolor=cpp_color, color=cpp_color),
        medianprops=dict(color=median_color)
        )
    plt.boxplot(
        x[1::2], positions=eff_pos[1::2],
        patch_artist=True,
        boxprops=dict(facecolor=rust_color, color=rust_color),
        medianprops=dict(color=median_color)
        )

    ax.set_xticks(pos)
    ax.set_xticklabels(tick_labels, fontsize=15)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=12)
    plt.ylabel(r'Time $[\mathrm{ms}]$', fontsize=15)
    plt.yscale('log')

    cpp_patch = mpatches.Patch(color=cpp_color, label='C++')
    rust_patch = mpatches.Patch(color=rust_color, label='Rust')
    ax.legend(handles=[cpp_patch, rust_patch], fontsize=15)

    plt.show()


def main():
    benchmarks = run_benchmarks()
    plot_benchmarks(benchmarks)


if __name__ == '__main__':
    main()
