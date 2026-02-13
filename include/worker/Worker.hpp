#pragma once

#include <thread>
#include <atomic>
#include "queue/ThreadSafeQueue.hpp"
#include "event/Event.hpp"
#include "engine/RuleEngine.hpp"

class Worker {
public:
    Worker(ThreadSafeQueue<Event>& queue);
    ~Worker();

    void start();
    void stop();

private:
    void workLoop();

    ThreadSafeQueue<Event>& m_queue;
    RuleEngine m_engine;

    std::thread m_thread;
    std::atomic<bool> running;
};