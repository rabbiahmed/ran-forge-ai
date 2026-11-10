
// Test cases for the workload generator
#include "workload_generator.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <limits>
#include <stdexcept>

namespace ranforge::benchmarks {
namespace {

TEST(WorkloadGeneratorTest, GeneratesRequestedNumberOfUsers)
{
    const WorkloadConfig config{100, 42};
    const auto users = generate_users(config);

    EXPECT_EQ(users.size(), 100);
}

TEST(WorkloadGeneratorTest, SameSeedProducesIdenticalWorkloads)
{
    const WorkloadConfig config{100, 42};

    const auto first = generate_users(config);
    const auto second = generate_users(config);

    ASSERT_EQ(first.size(), second.size());

    for (std::size_t i = 0; i < first.size(); ++i) {
        EXPECT_EQ(first[i].id, second[i].id);
        EXPECT_EQ(first[i].cqi, second[i].cqi);
        EXPECT_EQ(first[i].buffer_bytes, second[i].buffer_bytes);
        EXPECT_FLOAT_EQ(
            first[i].average_throughput_mbps,
            second[i].average_throughput_mbps
        );
        EXPECT_FLOAT_EQ(
            first[i].achievable_rate_mbps,
            second[i].achievable_rate_mbps
        );
        EXPECT_FLOAT_EQ(first[i].qos_weight, second[i].qos_weight);
    }
}

TEST(WorkloadGeneratorTest, GeneratesValidUserParameters)
{
    const WorkloadConfig config{1000, 42};
    const auto users = generate_users(config);

    for (const auto& user : users) {
        EXPECT_GE(user.cqi, 1);
        EXPECT_LE(user.cqi, 15);

        EXPECT_GT(user.buffer_bytes, 0U);

        EXPECT_GT(user.average_throughput_mbps, 0.0F);
        EXPECT_LE(user.average_throughput_mbps, 100.0F);

        EXPECT_GE(user.achievable_rate_mbps, 5.0F);
        EXPECT_LE(user.achievable_rate_mbps, 150.0F);

        EXPECT_GE(user.qos_weight, 0.5F);
        EXPECT_LE(user.qos_weight, 2.0F);
    }
}

TEST(WorkloadGeneratorTest, GeneratesUniqueUserIds)
{
    const WorkloadConfig config{100, 42};
    const auto users = generate_users(config);

    ASSERT_EQ(users.size(), 100);

    for (std::size_t i = 0; i < users.size(); ++i) {
        EXPECT_EQ(users[i].id, i + 1);
    }
}

TEST(WorkloadGeneratorTest, RejectsUserCountExceedingUeIdRange)
{
    if constexpr (
        std::numeric_limits<std::size_t>::max() <=
        std::numeric_limits<UeId>::max()
    ) {
        GTEST_SKIP()
            << "size_t cannot represent a value beyond the UeId range";
    }

    const auto invalid_count =
        static_cast<std::size_t>(
            std::numeric_limits<UeId>::max()
        ) + 1U;

    const WorkloadConfig config{invalid_count, 42};

    EXPECT_THROW(
        generate_users(config),
        std::invalid_argument
    );
}

TEST(WorkloadGeneratorTest, HandlesZeroUsers)
{
    const WorkloadConfig config{0, 42};
    const auto users = generate_users(config);

    EXPECT_TRUE(users.empty());
}

} // namespace
} // namespace ranforge::benchmarks
