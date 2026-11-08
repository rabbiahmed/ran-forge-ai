
// src/scheduler/proportional_fair.cpp
#include "ranforge/scheduler/proportional_fair.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>
#include <limits>

namespace ranforge {

namespace {

// Small positive value to prevent division by zero.
constexpr float kMinimumThroughput = 1.0e-6F;

// Internal representation of an eligible scheduling candidate.
struct SchedulingCandidate {
    UeId ue_id;
    float metric;
};

} // namespace

std::vector<PrbAllocation> ProportionalFairScheduler::schedule(
    const std::vector<UE>& users,
    std::uint32_t available_prbs)
{
    std::vector<PrbAllocation> allocations;

    // Nothing to schedule.
    if (users.empty() || available_prbs == 0) {
        return allocations;
    }

    std::vector<SchedulingCandidate> candidates;
    candidates.reserve(users.size());

    // Step 1: Filter invalid or ineligible UEs and calculate metrics.
    for (const UE& ue : users) {

        // Ignore UEs without data waiting for transmission.
        if (ue.buffer_bytes == 0) {
            continue;
        }

        // Reject invalid scheduling inputs.
        if (!std::isfinite(ue.average_throughput_mbps) ||
            !std::isfinite(ue.achievable_rate_mbps) ||
            !std::isfinite(ue.qos_weight)) {
            continue;
        }

        if (ue.average_throughput_mbps < 0.0F ||
            ue.achievable_rate_mbps <= 0.0F ||
            ue.qos_weight <= 0.0F) {
            continue;
        }

        // Step 2: Calculate the PF-inspired scheduling metric.
        const float denominator = std::max(
            ue.average_throughput_mbps,
            kMinimumThroughput
        );

        const double metric =
            (static_cast<double>(ue.achievable_rate_mbps) *
            static_cast<double>(ue.qos_weight)) /
            static_cast<double>(denominator);

        // Reject metrics that overflow or become non-finite.
        if (!std::isfinite(metric) ||
            metric > static_cast<double>(
                std::numeric_limits<float>::max())) {
            continue;
        }

        candidates.push_back({
            ue.id,
            static_cast<float>(metric)
        });
    }

    // No eligible UEs.
    if (candidates.empty()) {
        return allocations;
    }

    // Step 3: Rank candidates by descending scheduling metric.
    std::sort(
        candidates.begin(),
        candidates.end(),
        [](const SchedulingCandidate& lhs,
           const SchedulingCandidate& rhs) {

            if (lhs.metric != rhs.metric) {
                return lhs.metric > rhs.metric;
            }

            // Deterministic tie-breaking by UE ID.
            return lhs.ue_id < rhs.ue_id;
        }
    );

    // Step 4: Allocate PRBs in ranked round-robin order.
    allocations.reserve(available_prbs);

    for (std::uint32_t prb = 0; prb < available_prbs; ++prb) {

        const SchedulingCandidate& candidate =
            candidates[prb % candidates.size()];

        allocations.push_back({
            .prb_index = prb,
            .ue_id = candidate.ue_id,
            .scheduling_metric = candidate.metric
        });
    }

    return allocations;
}

} // namespace ranforge
