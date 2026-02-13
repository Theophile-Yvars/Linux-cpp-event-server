#pragma once

#include <cstdint>
#include <iostream>

struct CPUData {
    uint64_t user = 0, nice = 0, system = 0, idle = 0;
    uint64_t iowait = 0, irq = 0, softirq = 0, steal = 0;
    uint64_t guest = 0, guest_nice = 0;

    uint64_t total() const {
        return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
    }

    uint64_t idleTime() const {
        return idle + iowait;
    }

    // Print all raw CPU counters
    void print() const {
        std::cout << "CPU Data: "
                  << "user=" << user << " "
                  << "nice=" << nice << " "
                  << "system=" << system << " "
                  << "idle=" << idle << " "
                  << "iowait=" << iowait << " "
                  << "irq=" << irq << " "
                  << "softirq=" << softirq << " "
                  << "steal=" << steal << " "
                  << "guest=" << guest << " "
                  << "guest_nice=" << guest_nice
                  << std::endl;
    }
};

