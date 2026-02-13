#pragma once

#include <thread>
#include <atomic>
#include "queue/ThreadSafeQueue.hpp"
#include "event/Event.hpp"

class CPUMonitorThread {
public:
    explicit CPUMonitorThread(ThreadSafeQueue<Event>& queue);
    ~CPUMonitorThread();

    void start();
    void stop();

private:
    void monitorLoop();

    ThreadSafeQueue<Event>& m_threadSafeQueue;
    std::thread m_thread;
    std::atomic<bool> running;
};