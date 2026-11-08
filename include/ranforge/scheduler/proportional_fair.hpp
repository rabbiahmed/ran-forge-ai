
#pragma once

#include "ranforge/scheduler/scheduler.hpp"

namespace ranforge {

class ProportionalFairScheduler final : public Scheduler {
public:
    std::vector<PrbAllocation> schedule(
        const std::vector<UE>& users,
        std::uint32_t available_prbs
    ) override;
};

} // namespace ranforge
