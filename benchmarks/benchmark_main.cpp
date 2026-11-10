
// Benchmark for the proportional fair scheduler
#include <benchmark/benchmark.h>

#include "ranforge/scheduler/proportional_fair.hpp"
#include "workload_generator.hpp"

#include <cstddef>
#include <cstdint>

namespace {

void BM_ProportionalFairScheduler(benchmark::State& state)
{
    const auto ue_count = static_cast<std::size_t>(state.range(0));
    const auto available_prbs =
        static_cast<std::uint32_t>(state.range(1));

    const ranforge::benchmarks::WorkloadConfig config{
        ue_count,
        42
    };

    // Generate the workload outside the timed region.
    const auto users = ranforge::benchmarks::generate_users(config);

    ranforge::ProportionalFairScheduler scheduler;

    for (auto _ : state) {
        const auto allocations = scheduler.schedule(
            users,
            available_prbs
        );

        benchmark::DoNotOptimize(allocations.data());
        benchmark::DoNotOptimize(allocations.size());
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(
        state.iterations() * static_cast<int64_t>(ue_count)
    );
}

BENCHMARK(BM_ProportionalFairScheduler)
    ->Args({16, 25})
    ->Args({64, 100})
    ->Args({256, 273})
    ->Args({1024, 273})
    ->Args({4096, 273})
    ->Unit(benchmark::kMicrosecond);

} // namespace

BENCHMARK_MAIN();
