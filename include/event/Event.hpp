#pragma once
#include <chrono>
#include "EventType.hpp"

struct Event {
    EventType type;
    int value;
    std::chrono::system_clock::time_point timestamp;
};
 