#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>

#include "network/EpollLoopThread.hpp"
#include "queue/ThreadSafeQueue.hpp"
#include "worker/Worker.hpp"
#include "mock/EventProduction.hpp"
#include "event/Event.hpp"
#include "event/EventType.hpp"
#include "monitoring/CPUMonitorThread.hpp"
#include "mock/IProducer.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "=== Engine Starting ===" << endl;

    ThreadSafeQueue<Event> queue;

    EpollLoopThread epollLoop(queue);

    // CONSUMERS
    vector<unique_ptr<Worker>> workers;
    for (int i = 0; i < 3; i++) {
        workers.push_back(make_unique<Worker>(queue));
    }

    // PRODUCERS
    vector<unique_ptr<IProducer>> producers;
    producers.push_back(make_unique<EventProduction>(EventType::GO1));
    producers.push_back(make_unique<EventProduction>(EventType::GO2));
    producers.push_back(make_unique<EventProduction>(EventType::GO3));
    producers.push_back(make_unique<EventProduction>(EventType::GO4));
    producers.push_back(std::make_unique<CPUMonitorThread>(queue, EventType::CPU_OVERLOAD));

    epollLoop.start();

    for (auto& worker : workers)
        worker->start();

    for (auto& producer : producers)
        producer->start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    cout << "=== Stopping Producers ===" << endl;
    for (auto& producer : producers)
        producer->stop();

    cout << "=== Stopping Epoll Loop ===" << endl;
    epollLoop.stop();

    for (int i = 0; i < 3; i++) {
        queue.push(Event{
            EventType::SHUTDOWN,
            0,
            std::chrono::system_clock::now()
        });
    }

    for (auto& worker : workers)
        worker->stop();

    cout << "=== Engine Stopped Cleanly ===" << endl;

    return EXIT_SUCCESS;
}