#pragma once
#include "queue/ThreadSafeQueue.hpp"
#include "event/Event.hpp"
#include <thread>

class EpollLoopThread{
public:
    EpollLoopThread(ThreadSafeQueue<Event>& queue);
    ~EpollLoopThread();
    void start();
    void stop();
private:
    void _epollLoop();
    ThreadSafeQueue<Event>& _queue;
    bool _running;
    std::thread _thread;
};