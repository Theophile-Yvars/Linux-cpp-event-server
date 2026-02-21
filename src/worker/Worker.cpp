#include "worker/Worker.hpp"

Worker::Worker(ThreadSafeQueue<Event>& queue)
    : m_queue(queue)
{
}

Worker::~Worker() {
    stop();
}

void Worker::start() {
    m_thread = std::thread(&Worker::workLoop, this);
}

void Worker::stop() {
    if (m_thread.joinable())
        m_thread.join();
}

void Worker::workLoop() {
    while (true) {
        // 🔹 Attente bloquante d’un événement
        Event event = m_queue.pop();

        // 🔥 Condition d’arrêt propre
        if (event.type == EventType::SHUTDOWN)
            break;

        // 🔹 Traitement normal
        m_engine.process(event);
    }
}