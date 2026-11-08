
cmake_minimum_required(VERSION 3.22)

project(
    RANForge
    VERSION 0.1.0
    DESCRIPTION "GPU-Accelerated RAN Scheduling and Performance Engineering"
    LANGUAGES CXX
)

# GoogleTest dependency
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(googletest)

# Enable CTest
enable_testing()

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

# Core library
add_library(ranforge_core
    src/scheduler/proportional_fair.cpp
)

target_include_directories(ranforge_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
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
)

target_link_libraries(ranforge_tests
    PRIVATE
        ranforge_core
        GTest::gtest_main
)

# Automatically discover GoogleTest cases
include(GoogleTest)
gtest_discover_tests(ranforge_tests)
