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
    bool _running;
    std::thread _thread;
    int _epollFd;
    int _serverFd;
    ThreadSafeQueue<Event>& _queue;

    void _epollLoop();
};