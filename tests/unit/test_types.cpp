
#include <gtest/gtest.h>

#include "ranforge/core/types.hpp"

TEST(UEModelTest, InitializesFieldsCorrectly)
{
    ranforge::UE ue{
        .id = 101,
        .cqi = 12,
        .buffer_bytes = 250000,
        .average_throughput_mbps = 15.5F,
        .achievable_rate_mbps = 80.0F
    };

    EXPECT_EQ(ue.id, 101);
    EXPECT_EQ(ue.cqi, 12);
    EXPECT_EQ(ue.buffer_bytes, 250000);
    EXPECT_FLOAT_EQ(ue.average_throughput_mbps, 15.5F);
    EXPECT_FLOAT_EQ(ue.achievable_rate_mbps, 80.0F);
    EXPECT_FLOAT_EQ(ue.qos_weight, 1.0F);
}

TEST(UEModelTest, DefaultInitialization)
{
    ranforge::UE ue{};

    EXPECT_EQ(ue.id, 0);
    EXPECT_EQ(ue.cqi, 0);
    EXPECT_EQ(ue.buffer_bytes, 0);
    EXPECT_FLOAT_EQ(ue.average_throughput_mbps, 0.0F);
    EXPECT_FLOAT_EQ(ue.achievable_rate_mbps, 0.0F);
    EXPECT_FLOAT_EQ(ue.qos_weight, 1.0F);
}

TEST(PrbAllocationTest, InitializesFieldsCorrectly)
{
    ranforge::PrbAllocation allocation{
        .prb_index = 5,
        .ue_id = 101,
        .scheduling_metric = 0.75F
    };

    EXPECT_EQ(allocation.prb_index, 5);
    EXPECT_EQ(allocation.ue_id, 101);
    EXPECT_FLOAT_EQ(allocation.scheduling_metric, 0.75F);
}

TEST(PrbAllocationTest, DefaultInitialization)
{
    ranforge::PrbAllocation allocation{};

    EXPECT_EQ(allocation.prb_index, 0);
    EXPECT_EQ(allocation.ue_id, 0);
    EXPECT_FLOAT_EQ(allocation.scheduling_metric, 0.0F);
}
