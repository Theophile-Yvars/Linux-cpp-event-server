#pragma once
#include "event/Event.hpp"

class RuleEngine {
public:
    void process(const Event& event);
};