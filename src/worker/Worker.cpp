#include "worker/Worker.hpp"

Worker::Worker(ThreadSafeQueue<Event>& queue)
    : m_queue(queue), running(false) {}

Worker::~Worker() {
    stop();
}

void Worker::start() {
    running = true;
    m_thread = std::thread(&Worker::workLoop, this);
}

void Worker::stop() {
    running = false;
    if (m_thread.joinable())
        m_thread.join();
}

void Worker::workLoop() {
    while (running) {
        Event event = m_queue.pop(); // blocking
        m_engine.process(event);
        if (event.type == EventType::SHUTDOWN)
            break;
    }
}