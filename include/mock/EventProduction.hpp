#pragma once
#include <thread>
#include "event/EventType.hpp"
#include "IProducer.hpp"

class EventProduction : public IProducer {
public:
    EventProduction(EventType eventType);
    ~EventProduction();
    void start() override;
    void stop() override;
private:
    void produceEvent();
    bool _running;
    std::thread _thread;
    EventType _eventType;
};