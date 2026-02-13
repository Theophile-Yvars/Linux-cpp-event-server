#include <iostream>
#include <thread>
#include <chrono>
#include "monitoring/CPUMonitorThread.hpp"
#include "queue/ThreadSafeQueue.hpp"
#include "worker/Worker.hpp"

using namespace std;

int main( int argc, char* argv[]) {
    cout << "Hello, World!" << endl;
    
    ThreadSafeQueue<Event> queue;
    CPUMonitorThread monitor(queue);
    Worker worker(queue);

    monitor.start();
    worker.start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    monitor.stop();    
    queue.push(Event{
        EventType::SHUTDOWN,
        0,
        std::chrono::system_clock::now()
    });
    worker.stop();

    return EXIT_SUCCESS;
}