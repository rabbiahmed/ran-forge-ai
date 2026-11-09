# RANForge

**GPU-Accelerated RAN Scheduling and Performance Engineering**

RANForge is an experimental C++20 project focused on radio access network (RAN) scheduling, systems programming, and GPU performance engineering.

The project takes an incremental, measurement-driven approach to exploring how modern C++ and GPU computing can be applied to wireless resource scheduling workloads.

The initial implementation establishes a CPU-based scheduling foundation that will serve as a reference for future performance experiments and GPU implementations.

## Project Objectives

- Implement and evaluate RAN resource scheduling algorithms in modern C++.
- Develop a reusable, modular scheduler architecture.
- Establish CPU performance baselines using representative scheduling workloads.
- Explore GPU acceleration with NVIDIA CUDA.
- Compare CPU and GPU implementations through reproducible benchmarks.
- Apply systems engineering practices, including unit testing, profiling, and performance analysis.

## Current Status

**Implemented: CPU proportional-fair-inspired scheduler**

The current implementation includes:

- A basic UE (User Equipment) domain model.
- A PRB (Physical Resource Block) allocation model.
- An abstract scheduler interface.
- A CPU-based proportional-fair-inspired scheduler.
- Input validation and deterministic tie-breaking.
- Round-robin PRB distribution across ranked scheduling candidates.
- Unit tests covering normal, boundary, and invalid-input scenarios.

The scheduler uses the following metric:

$$
M_i = \frac{R_i\times W_i}{\max(T_i,\epsilon)}
$$

Where:

- \(R_i\): Achievable data rate for UE \(i\).
- \(W_i\): QoS weight for UE \(i\).
- \(T_i\): Average throughput for UE \(i\).
- \(\epsilon\): Small positive value to avoid division by zero.

Candidates are ranked by their scheduling metric, and available PRBs are distributed in ranked round-robin order.

**Scope clarification:** This is a proportional-fair-inspired baseline for experimentation. It is not a complete 3GPP-compliant MAC scheduler or a full dynamic proportional-fair implementation.

## Technology Stack

| Component | Technology |
|---|---|
| Programming language | C++20 |
| Build system | CMake |
| Build generator | Ninja |
| Compiler | GCC |
| Unit testing | GoogleTest |
| Version control | Git / GitHub |
| Planned GPU development | NVIDIA CUDA |
| Planned profiling | NVIDIA Nsight Systems and Nsight Compute |

## Project Structure

```text
ranforge/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── include/
│   └── ranforge/
│       ├── core/
│       │   └── types.hpp
│       └── scheduler/
│           ├── scheduler.hpp
│           └── proportional_fair.hpp
├── src/
│   ├── main.cpp
│   └── scheduler/
│       └── proportional_fair.cpp
└── tests/
    └── unit/
        ├── test_types.cpp
        └── test_scheduler.cpp
```

## Build Instructions

### Prerequisites

- Linux development environment
- GCC 11 or compatible C++20 compiler
- CMake 3.22 or later
- Ninja
- Git

GoogleTest is fetched automatically through CMake FetchContent during configuration, so an internet connection is required for the initial dependency download.

### Configure

```bash
cmake -S . -B build \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=g++-11
```

### Build

```bash
cmake --build build -j2
```

### Run the application

```bash
./build/ranforge
```

### Run unit tests

```bash
ctest --test-dir build --output-on-failure
```

## Development Roadmap

The project is being developed incrementally, with correctness and measurable performance as primary goals.

| Phase | Focus | Status |
|---|---|---|
| 1 | C++ project foundation and UE/PRB domain models | Completed |
| 2 | CPU scheduler abstraction and proportional-fair-inspired baseline | Completed |
| 3 | Scheduler integration and benchmarking framework | Planned |
| 4 | CPU performance analysis and workload scaling | Planned |
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
- **Incremental development:** Use focused changes, feature branches, and pull requests.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

*RANForge is an independent engineering and experimentation project exploring performance-oriented software design for wireless systems.*