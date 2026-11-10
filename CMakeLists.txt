
# CMakeLists.txt for RANForge project
cmake_minimum_required(VERSION 3.22)

project(
    RANForge
    VERSION 0.1.0
    DESCRIPTION "GPU-Accelerated RAN Scheduling and Performance Engineering"
    LANGUAGES CXX
)

# External dependencies
include(FetchContent)

# GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(googletest)

# Google Benchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.5
)

FetchContent_MakeAvailable(googlebenchmark)

# Enable CTest
enable_testing()

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core library
add_library(ranforge_core
    src/scheduler/proportional_fair.cpp
)

target_include_directories(ranforge_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_library(ranforge_benchmark_support
    benchmarks/workload_generator.cpp
)

target_include_directories(ranforge_benchmark_support
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/benchmarks
)

target_link_libraries(ranforge_benchmark_support
    PUBLIC
        ranforge_core
)

# CPU scheduler benchmarks
add_executable(ranforge_benchmarks
    benchmarks/benchmark_main.cpp
)

target_link_libraries(ranforge_benchmarks
    PRIVATE
        ranforge_core
        ranforge_benchmark_support
        benchmark::benchmark
)

# Main application
add_executable(ranforge
    src/main.cpp
)

target_link_libraries(ranforge
    PRIVATE
        ranforge_core
)

# Unit tests
add_executable(ranforge_tests
    tests/unit/test_types.cpp
    tests/unit/test_scheduler.cpp
    tests/unit/test_workload_generator.cpp
)

target_link_libraries(ranforge_tests
    PRIVATE
        ranforge_core
        ranforge_benchmark_support
        GTest::gtest_main
)

# Automatically discover GoogleTest cases
include(GoogleTest)
gtest_discover_tests(ranforge_tests)

# Compiler warnings for RANForge targets
foreach(target
    ranforge_core
    ranforge_benchmark_support
    ranforge_benchmarks
    ranforge
    ranforge_tests
)
    target_compile_options(${target}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endforeach()
