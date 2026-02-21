#include "engine/RuleEngine.hpp"
#include <iostream>

void RuleEngine::process(const Event& event) {
    switch (event.type) {

        case EventType::CPU_OVERLOAD:
            if (event.value > 0.16) {
                std::cout << "[ENGINE] CRITICAL CPU overload: "
                          << event.value << "%\n";
            } else {
                std::cout << "[ENGINE] High CPU: "
                          << event.value << "%\n";
            }
            break;
        
        case EventType::GO1:
            std::cout << "[ENGINE] Received event GO1 with value " << event.value << "\n";
            break;

        case EventType::GO2:
            std::cout << "[ENGINE] Received event GO2 with value " << event.value << "\n";
            break;

        case EventType::GO3:
            std::cout << "[ENGINE] Received event GO3 with value " << event.value << "\n";
            break;

        case EventType::GO4:
            std::cout << "[ENGINE] Received event GO4 with value " << event.value << "\n";
            break;

        default:
            break;
    }
}