
// Workload generator for benchmarking
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "ranforge/core/types.hpp"

namespace ranforge::benchmarks {

struct WorkloadConfig {
    std::size_t ue_count{};
    std::uint32_t seed{42};
};

std::vector<UE> generate_users(const WorkloadConfig& config);

} // namespace ranforge::benchmarks
