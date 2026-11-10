# RANForge

**GPU-Accelerated RAN Scheduling and Performance Engineering**

RANForge is an experimental C++20 project focused on radio access network (RAN) scheduling, systems programming, and GPU performance engineering.

The project explores how modern C++ and GPU computing can be applied to wireless resource scheduling workloads through an incremental, measurement-driven engineering approach.

The initial implementation establishes a CPU-based scheduling foundation, deterministic workload generation, and a reproducible benchmarking framework. These components provide a reference for future performance experiments and GPU implementations.

## Project Objectives

- Implement and evaluate RAN resource scheduling algorithms in modern C++.
- Develop a reusable, modular scheduler architecture.
- Establish CPU performance baselines using representative scheduling workloads.
- Explore GPU acceleration with NVIDIA CUDA.
- Compare CPU and GPU implementations through reproducible benchmarks.
- Apply systems engineering practices, including unit testing, profiling, and performance analysis.

## Current Status

**Implemented: CPU Scheduler and Benchmarking Framework**

The current implementation includes:

- A basic UE (User Equipment) domain model.
- A PRB (Physical Resource Block) allocation model.
- An abstract scheduler interface.
- A CPU-based proportional-fair-inspired scheduler.
- Input validation and deterministic tie-breaking.
- Round-robin PRB distribution across ranked scheduling candidates.
- Deterministic synthetic UE workload generation using configurable random seeds.
- Google Benchmark integration.
- Multiple workload configurations for evaluating scheduler performance.
- Unit tests covering domain models, scheduling behavior, and workload generation.
- Initial Release-mode CPU performance measurements and environment metadata.

### CPU Scheduling Algorithm

The scheduler uses the following metric:

$$
M_i = \frac{R_i \times W_i}{\max(T_i,\epsilon)}
$$

Where:

- \(R_i\): Achievable data rate for UE \(i\).
- \(W_i\): QoS weight for UE \(i\).
- \(T_i\): Average throughput for UE \(i\).
- \(\epsilon\): Small positive value to avoid division by zero.

Candidates are ranked by their scheduling metric, and available PRBs are distributed in ranked round-robin order.

**Scope clarification:** This is a proportional-fair-inspired baseline for experimentation. It is not a complete 3GPP-compliant MAC scheduler or a full dynamic proportional-fair implementation.

### CPU Benchmarking Framework

RANForge uses Google Benchmark to measure the CPU scheduler under different synthetic workload sizes.

The workload generator creates deterministic UE datasets using a configurable seed. Workload generation occurs outside the timed benchmark loop so that measurements focus on scheduler execution rather than random-number generation.

The benchmark measures the complete scheduler call, including construction of the resulting PRB allocation vector.

Current benchmark configurations:

| UE count | Available PRBs |
|---:|---:|
| 16 | 25 |
| 64 | 100 |
| 256 | 273 |
| 1,024 | 273 |
| 4,096 | 273 |

The initial baseline uses five repetitions and a minimum measurement time of 0.5 seconds per repetition.

## CPU Performance Baseline

The following measurements were collected using a Release build on the development machine.

| UE count | PRBs | Mean latency (µs) | Median latency (µs) | UE throughput (M/s) |
|---:|---:|---:|---:|---:|
| 16 | 25 | 0.348 | 0.348 | 45.99 |
| 64 | 100 | 1.514 | 1.505 | 42.28 |
| 256 | 273 | 5.501 | 5.514 | 46.54 |
| 1,024 | 273 | 36.686 | 36.702 | 27.91 |
| 4,096 | 273 | 205.378 | 205.301 | 19.95 |

UE throughput is calculated from Google Benchmark's `items_per_second` metric, where each processed item represents one UE in a scheduler invocation. It is not the number of scheduler invocations per second.

### Benchmark Environment

| Component | Specification |
|---|---|
| Operating system | Ubuntu 22.04.5 LTS |
| Architecture | x86_64 |
| CPU | Intel Core i7-9750H @ 2.60 GHz |
| Logical CPUs | 12 |
| CPU frequency range | 800–4500 MHz |
| Memory | 16 GB RAM |
| Compiler | GCC 11.4.0 |
| Build system | CMake 3.22.1 |
| Build generator | Ninja |
| Build configuration | Release |
| Benchmark library | Google Benchmark 1.9.5 |

**Measurement caveat:** CPU frequency scaling was enabled during the baseline run. These results should be treated as preliminary, machine-specific measurements rather than universal performance characteristics. More controlled measurements will be required for rigorous CPU/GPU comparisons.

The original benchmark data and environment metadata are maintained in the repository:

- `results/cpu_scheduler_baseline_clean.csv`
- `results/cpu_scheduler_environment.txt`

## Technology Stack

| Component | Technology |
|---|---|
| Programming language | C++20 |
| Build system | CMake |
| Build generator | Ninja |
| Compiler | GCC |
| Unit testing | GoogleTest |
| Benchmarking | Google Benchmark |
| Version control | Git / GitHub |
| Planned GPU development | NVIDIA CUDA |
| Planned profiling | NVIDIA Nsight Systems and Nsight Compute |

GoogleTest and Google Benchmark are fetched automatically through CMake FetchContent during configuration. An internet connection is therefore required for the initial dependency download.

## Project Structure

```text
ranforge/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── benchmarks/
│   ├── benchmark_main.cpp
│   ├── workload_generator.hpp
│   └── workload_generator.cpp
├── include/
│   └── ranforge/
│       ├── core/
│       │   └── types.hpp
│       └── scheduler/
│           ├── scheduler.hpp
│           └── proportional_fair.hpp
├── results/
│   ├── cpu_scheduler_baseline_clean.csv
│   └── cpu_scheduler_environment.txt
├── src/
│   ├── main.cpp
│   └── scheduler/
│       └── proportional_fair.cpp
└── tests/
    └── unit/
        ├── test_types.cpp
        ├── test_scheduler.cpp
        └── test_workload_generator.cpp
```

## Build Instructions

### Prerequisites

- Linux development environment.
- GCC 11 or compatible C++20 compiler.
- CMake 3.22 or later.
- Ninja.
- Git.
- Internet connection for initial dependency downloads.

### Configure a Release Build

```bash
cmake -S . -B build-release \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=g++-11
```

### Build

```bash
cmake --build build-release -j2
```

### Run the Application

```bash
./build-release/ranforge
```

### Run Unit Tests

```bash
ctest --test-dir build-release --output-on-failure
```

### Run CPU Benchmarks

```bash
./build-release/ranforge_benchmarks \
    --benchmark_repetitions=5 \
    --benchmark_min_time=0.5s \
    --benchmark_report_aggregates_only=true
```

### Export Benchmark Results

```bash
mkdir -p results

./build-release/ranforge_benchmarks \
    --benchmark_repetitions=5 \
    --benchmark_min_time=0.5s \
    --benchmark_report_aggregates_only=true \
    --benchmark_out=results/cpu_scheduler_baseline.csv \
    --benchmark_out_format=csv
```

Google Benchmark may include environment information before the CSV header in the output file. To extract the clean CSV data:

```bash
sed -n '/^name,iterations/,$p' \
    results/cpu_scheduler_baseline.csv \
    > results/cpu_scheduler_baseline_clean.csv
```

Record the compiler, build configuration, CPU specifications, operating system, benchmark version, and measurement conditions alongside any new baseline.

## Development Roadmap

The project is being developed incrementally, with correctness and measurable performance as primary goals.

| Phase | Focus | Status |
|---|---|---|
| 1 | C++ project foundation and UE/PRB domain models | Completed |
| 2 | CPU scheduler abstraction and proportional-fair-inspired baseline | Completed |
| 3 | Deterministic workload generation and benchmarking framework | Completed |
| 4 | CPU performance analysis and workload scaling | In progress |
| 5 | CUDA fundamentals and GPU implementation | Planned |
| 6 | CPU/GPU performance comparison and optimization | Planned |
| 7 | Profiling, analysis, and engineering documentation | Planned |

The CPU implementation will remain the reference against which future GPU implementations are evaluated.

GPU acceleration will be introduced only where measurements demonstrate a meaningful opportunity. Kernel execution time, memory-transfer overhead, and end-to-end performance will all be considered.

## Engineering Principles

- **Correctness first:** Establish a tested CPU reference before introducing GPU complexity.
- **Measure before optimizing:** Use repeatable benchmarks rather than assumptions about performance.
- **Modular design:** Keep scheduling algorithms and performance experiments independently testable.
- **Reproducibility:** Document configurations, workloads, and benchmark methodology.
- **Performance transparency:** Report measurement conditions, limitations, and relevant overheads.
- **Incremental development:** Use focused changes, feature branches, and pull requests.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

*RANForge is an independent engineering and experimentation project exploring performance-oriented software design for wireless systems.*