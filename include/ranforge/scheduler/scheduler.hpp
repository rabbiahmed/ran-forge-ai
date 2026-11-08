
#pragma once

#include <cstdint>
#include <vector>

#include "ranforge/core/types.hpp"

namespace ranforge {

class Scheduler {
public:
    // Virtual destructor ensures safe destruction of derived objects
    // through a base-class pointer.
    virtual ~Scheduler() = default;

    // Abstract base class for scheduling algorithms. Derived classes must implement the `schedule` method,
    // which takes a list of users and the number of available PRBs, and returns a vector of PRB allocations.
    virtual std::vector<PrbAllocation> schedule(
        const std::vector<UE>& users,
        std::uint32_t available_prbs
    ) = 0; // The = 0 makes this a pure virtual function.
};

} // namespace ranforge
