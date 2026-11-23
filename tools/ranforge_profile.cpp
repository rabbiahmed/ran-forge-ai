#include "ranforge/scheduler/proportional_fair.hpp"
#include "workload_generator.hpp"

#include <valgrind/callgrind.h>

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string_view>
#include <system_error>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: ranforge_profile <ue_count>\n";
        return EXIT_FAILURE;
    }

    // Parse a strictly positive UE count.
    const std::string_view input{argv[1]};
    std::size_t ue_count{};

    const auto [end, error] = std::from_chars(
        input.data(),
        input.data() + input.size(),
        ue_count
    );

    if (error != std::errc{} ||
        end != input.data() + input.size() ||
        ue_count == 0) {
        std::cerr << "Error: ue_count must be a positive integer.\n";
        return EXIT_FAILURE;
    }

    constexpr std::uint32_t available_prbs = 273;
    constexpr std::uint32_t seed = 42;

    try {
        const ranforge::benchmarks::WorkloadConfig config{
            .ue_count = ue_count,
            .seed = seed
        };

        // Generate the workload before profiling.
        const auto users =
            ranforge::benchmarks::generate_users(config);

        ranforge::ProportionalFairScheduler scheduler;

        // Collect instructions only during scheduler execution.
        CALLGRIND_TOGGLE_COLLECT;

        const auto allocations =
            scheduler.schedule(users, available_prbs);

        CALLGRIND_TOGGLE_COLLECT;

        std::cout << "UEs: " << users.size()
                  << ", PRBs: " << available_prbs
                  << ", allocations: " << allocations.size()
                  << '\n';

    } catch (const std::exception& exception) {
        std::cerr << "Profiling harness error: "
                  << exception.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
