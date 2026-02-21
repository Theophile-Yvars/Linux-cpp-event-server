#pragma once

#include <thread>
#include <atomic>
#include "queue/ThreadSafeQueue.hpp"
#include "event/Event.hpp"
#include "event/EventType.hpp"
#include "mock/IProducer.hpp"

class CPUMonitorThread : public IProducer {
public:
    explicit CPUMonitorThread(ThreadSafeQueue<Event>& queue, EventType eventType);
    ~CPUMonitorThread();

    void start() override;
    void stop() override;

private:
    void monitorLoop();

    ThreadSafeQueue<Event>& m_threadSafeQueue;
    std::thread m_thread;
    std::atomic<bool> running;
    EventType m_eventType;
};