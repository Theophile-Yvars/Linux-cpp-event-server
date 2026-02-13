#pragma once
#include <chrono>
#include "EventType.hpp"

struct Event {
    EventType type;
    float value;
    std::chrono::system_clock::time_point timestamp;
};
 