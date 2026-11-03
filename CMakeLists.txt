cmake_minimum_required(VERSION 3.22)

project(
    RANForge
    VERSION 0.1.0
    DESCRIPTION "GPU-Accelerated RAN Scheduling and Performance Engineering"
    LANGUAGES CXX
)

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

# Main application
add_executable(ranforge
    src/main.cpp
)

target_include_directories(ranforge
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
