// Workload generator implementation for benchmarking
#include "workload_generator.hpp"

#include <random>
#include <limits>
#include <stdexcept>

namespace ranforge::benchmarks {

std::vector<UE> generate_users(const WorkloadConfig& config)
{
    if (config.ue_count > std::numeric_limits<UeId>::max()) {
        throw std::invalid_argument(
            "UE count exceeds the maximum representable UE ID"
        );
    }

    std::mt19937 generator(config.seed);

    std::uniform_int_distribution<int> cqi_distribution(1, 15);
    std::uniform_int_distribution<std::uint64_t> buffer_distribution(
        10'000, 500'000
    );

    std::uniform_real_distribution<float> throughput_distribution(
        1.0F, 100.0F
    );

    std::uniform_real_distribution<float> rate_distribution(
        5.0F, 150.0F
    );

    std::uniform_real_distribution<float> qos_distribution(
        0.5F, 2.0F
    );

    std::vector<UE> users;
    users.reserve(config.ue_count);

    for (std::size_t i = 0; i < config.ue_count; ++i) {
        UE user{};

        user.id = static_cast<UeId>(i + 1);
        user.cqi = static_cast<std::uint8_t>(
            cqi_distribution(generator)
        );
        user.buffer_bytes = buffer_distribution(generator);
        user.average_throughput_mbps =
            throughput_distribution(generator);
        user.achievable_rate_mbps =
            rate_distribution(generator);
        user.qos_weight = qos_distribution(generator);

        users.push_back(user);
    }

    return users;
}

} // namespace ranforge::benchmarks