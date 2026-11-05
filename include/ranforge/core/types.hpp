#pragma once

#include <cstdint>

namespace ranforge {

// Unique identifier for a User Equipment (UE).
using UeId = std::uint32_t;

// Index identifying a Physical Resource Block (PRB).
using PrbIndex = std::uint32_t;

// Simplified UE state used by the RAN scheduler.
struct UE {
    UeId id{};

    // Channel Quality Indicator (CQI): typically 0-15.
    std::uint8_t cqi{};

    // Amount of data waiting to be transmitted.
    std::uint64_t buffer_bytes{};

    // Historical average throughput in Mbps.
    float average_throughput_mbps{};

    // Estimated achievable data rate in Mbps.
    float achievable_rate_mbps{};

    // Relative QoS scheduling weight.
    float qos_weight{1.0F};
};

// Records the assignment of a PRB to a UE.
struct PrbAllocation {
    PrbIndex prb_index{};
    UeId ue_id{};
    float scheduling_metric{};
};

} // namespace ranforge
