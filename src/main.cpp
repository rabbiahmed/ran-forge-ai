#include <iostream>
#include "ranforge/core/types.hpp"

int main()
{
    std::cout << "====================================\n";
    std::cout << "       RANForge v0.1.0\n";
    std::cout << "====================================\n";

    std::cout << "RAN Scheduling and Performance Engineering\n";
    std::cout << "C++20 environment initialized successfully.\n";

    ranforge::UE ue1{
        .id = 101,
        .cqi = 12,
        .buffer_bytes = 250000,
        .average_throughput_mbps = 15.5F,
        .achievable_rate_mbps = 80.0F
    };

    ranforge::PrbAllocation alloc1{
        .prb_index = 5,
        .ue_id = ue1.id,
        .scheduling_metric = 0.75F
    };

    std::cout << "UE ID: " << ue1.id << '\n';
    std::cout << "CQI: " << static_cast<unsigned int>(ue1.cqi) << '\n';
    std::cout << "Buffer: " << ue1.buffer_bytes << " bytes\n";
    std::cout << "Average Throughput: " << ue1.average_throughput_mbps << " Mbps\n";
    std::cout << "Achievable Rate: " << ue1.achievable_rate_mbps << " Mbps\n";
    std::cout << "QoS Weight: " << ue1.qos_weight << '\n';
    std::cout << "\nPRB Index: " << alloc1.prb_index << '\n';
    std::cout << "Allocated to UE: " << alloc1.ue_id << '\n';
    
    return 0;
}
