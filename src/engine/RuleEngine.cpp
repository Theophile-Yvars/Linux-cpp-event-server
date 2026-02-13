#include "engine/RuleEngine.hpp"
#include <iostream>

void RuleEngine::process(const Event& event) {
    std::cout<<"ENGINE EVENT: " << std::endl;
    switch (event.type) {

        case EventType::CPU_OVERLOAD:
            if (event.value > 90.0) {
                std::cout << "[ENGINE] CRITICAL CPU overload: "
                          << event.value << "%\n";
            } else {
                std::cout << "[ENGINE] High CPU: "
                          << event.value << "%\n";
            }
            break;

        default:
            break;
    }
}