#pragma once
#include <thread>
#include "EventType.hpp"

class EventProduction {
public:
    EventProduction(EventType eventType);
    ~EventProduction();
    void start();
    void stop();
private:
    void produceEvent();
    bool _running;
    std::thread _thread;
    EventType _eventType;
};