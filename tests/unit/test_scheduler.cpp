
// tests/unit/test_scheduler.cpp
#include <gtest/gtest.h>

#include <vector>

#include "ranforge/scheduler/proportional_fair.hpp"

#include <limits>

namespace ranforge {

TEST(ProportionalFairSchedulerTest, EmptyUsersReturnNoAllocations)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users;

    const auto allocations = scheduler.schedule(users, 10);

    EXPECT_TRUE(allocations.empty());
}

TEST(ProportionalFairSchedulerTest, ZeroPrbsReturnNoAllocations)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users{
        {.id = 101,
         .cqi = 12,
         .buffer_bytes = 250000,
         .average_throughput_mbps = 15.5F,
         .achievable_rate_mbps = 80.0F,
         .qos_weight = 1.0F}
    };

    const auto allocations = scheduler.schedule(users, 0);

    EXPECT_TRUE(allocations.empty());
}

TEST(ProportionalFairSchedulerTest, UesWithEmptyBuffersAreExcluded)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users{
        {.id = 101,
         .cqi = 12,
         .buffer_bytes = 0,
         .average_throughput_mbps = 15.5F,
         .achievable_rate_mbps = 80.0F,
         .qos_weight = 1.0F}
    };

    const auto allocations = scheduler.schedule(users, 5);

    EXPECT_TRUE(allocations.empty());
}

TEST(ProportionalFairSchedulerTest, AllocatesAllAvailablePrbs)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users{
        {.id = 101,
         .cqi = 12,
         .buffer_bytes = 250000,
         .average_throughput_mbps = 15.5F,
         .achievable_rate_mbps = 80.0F,
         .qos_weight = 1.0F},

        {.id = 102,
         .cqi = 10,
         .buffer_bytes = 150000,
         .average_throughput_mbps = 20.0F,
         .achievable_rate_mbps = 60.0F,
         .qos_weight = 1.0F}
    };

    constexpr std::uint32_t available_prbs = 6;

    const auto allocations =
        scheduler.schedule(users, available_prbs);

    ASSERT_EQ(allocations.size(), available_prbs);

    for (std::uint32_t i = 0; i < available_prbs; ++i) {
        EXPECT_EQ(allocations[i].prb_index, i);
    }
}

TEST(ProportionalFairSchedulerTest, HighestMetricGetsFirstPrb)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users{
        {.id = 101,
         .cqi = 12,
         .buffer_bytes = 250000,
         .average_throughput_mbps = 15.5F,
         .achievable_rate_mbps = 80.0F,
         .qos_weight = 1.0F},

        {.id = 102,
         .cqi = 10,
         .buffer_bytes = 150000,
         .average_throughput_mbps = 20.0F,
         .achievable_rate_mbps = 60.0F,
         .qos_weight = 1.0F},

        {.id = 103,
         .cqi = 8,
         .buffer_bytes = 100000,
         .average_throughput_mbps = 5.0F,
         .achievable_rate_mbps = 30.0F,
         .qos_weight = 1.5F}
    };

    const auto allocations = scheduler.schedule(users, 3);

    ASSERT_EQ(allocations.size(), 3);

    // UE 103 has the highest metric:
    // (30 * 1.5) / 5 = 9.0
    EXPECT_EQ(allocations[0].ue_id, 103);
}

TEST(ProportionalFairSchedulerTest, DistributesPrbsInRoundRobinOrder)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users{
        {.id = 101,
         .cqi = 12,
         .buffer_bytes = 250000,
         .average_throughput_mbps = 15.5F,
         .achievable_rate_mbps = 80.0F,
         .qos_weight = 1.0F},

        {.id = 102,
         .cqi = 10,
         .buffer_bytes = 150000,
         .average_throughput_mbps = 20.0F,
         .achievable_rate_mbps = 60.0F,
         .qos_weight = 1.0F}
    };

    const auto allocations = scheduler.schedule(users, 4);

    ASSERT_EQ(allocations.size(), 4);

    // UE 101 has a higher metric than UE 102.
    EXPECT_EQ(allocations[0].ue_id, 101);
    EXPECT_EQ(allocations[1].ue_id, 102);
    EXPECT_EQ(allocations[2].ue_id, 101);
    EXPECT_EQ(allocations[3].ue_id, 102);
}

TEST(ProportionalFairSchedulerTest, InvalidSchedulingInputsAreExcluded)
{
    ProportionalFairScheduler scheduler;

    const float nan = std::numeric_limits<float>::quiet_NaN();
    const float infinity = std::numeric_limits<float>::infinity();

    const std::vector<UE> users{
        // Invalid: NaN throughput
        {.id = 101, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = nan,
         .achievable_rate_mbps = 80.0F, .qos_weight = 1.0F},

        // Invalid: infinite achievable rate
        {.id = 102, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 10.0F,
         .achievable_rate_mbps = infinity, .qos_weight = 1.0F},

        // Invalid: negative throughput
        {.id = 103, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = -1.0F,
         .achievable_rate_mbps = 80.0F, .qos_weight = 1.0F},

        // Invalid: zero achievable rate
        {.id = 104, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 10.0F,
         .achievable_rate_mbps = 0.0F, .qos_weight = 1.0F},

        // Invalid: zero QoS weight
        {.id = 105, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 10.0F,
         .achievable_rate_mbps = 80.0F, .qos_weight = 0.0F},

        // Valid UE
        {.id = 106, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 10.0F,
         .achievable_rate_mbps = 80.0F, .qos_weight = 1.0F}
    };

    const auto allocations = scheduler.schedule(users, 3);

    ASSERT_EQ(allocations.size(), 3);

    for (const auto& allocation : allocations) {
        EXPECT_EQ(allocation.ue_id, 106);
    }
}

TEST(ProportionalFairSchedulerTest, EqualMetricsUseUeIdAsTieBreaker)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users{
        {.id = 202, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 10.0F,
         .achievable_rate_mbps = 50.0F, .qos_weight = 1.0F},

        {.id = 101, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 10.0F,
         .achievable_rate_mbps = 50.0F, .qos_weight = 1.0F}
    };

    const auto allocations = scheduler.schedule(users, 2);

    ASSERT_EQ(allocations.size(), 2);

    EXPECT_EQ(allocations[0].ue_id, 101);
    EXPECT_EQ(allocations[1].ue_id, 202);
}


TEST(ProportionalFairSchedulerTest, ZeroAverageThroughputRemainsEligible)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users{
        {.id = 101, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 0.0F,
         .achievable_rate_mbps = 10.0F,
         .qos_weight = 1.0F}
    };

    const auto allocations = scheduler.schedule(users, 1);

    ASSERT_EQ(allocations.size(), 1);
    EXPECT_EQ(allocations[0].ue_id, 101);

    // 10 / 1e-6 = 10,000,000
    EXPECT_FLOAT_EQ(
        allocations[0].scheduling_metric,
        10'000'000.0F
    );
}

TEST(ProportionalFairSchedulerTest, OverflowingMetricIsExcluded)
{
    ProportionalFairScheduler scheduler;

    const float largest_float =
        std::numeric_limits<float>::max();

    const std::vector<UE> users{
        // Inputs are finite, but their product exceeds float range.
        {.id = 101, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 1.0F,
         .achievable_rate_mbps = largest_float,
         .qos_weight = largest_float},

        // Valid UE.
        {.id = 102, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 10.0F,
         .achievable_rate_mbps = 50.0F,
         .qos_weight = 1.0F}
    };

    const auto allocations = scheduler.schedule(users, 2);

    ASSERT_EQ(allocations.size(), 2);

    for (const auto& allocation : allocations) {
        EXPECT_EQ(allocation.ue_id, 102);
    }
}

TEST(ProportionalFairSchedulerTest, AllInvalidUsersReturnNoAllocations)
{
    ProportionalFairScheduler scheduler;

    const std::vector<UE> users{
        {.id = 101, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = -1.0F,
         .achievable_rate_mbps = 50.0F,
         .qos_weight = 1.0F},

        {.id = 102, .cqi = 12, .buffer_bytes = 1000,
         .average_throughput_mbps = 10.0F,
         .achievable_rate_mbps = 50.0F,
         .qos_weight = 0.0F}
    };

    const auto allocations = scheduler.schedule(users, 5);

    EXPECT_TRUE(allocations.empty());
}

} // namespace ranforge
