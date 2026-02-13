#include "monitoring/CPUMonitorThread.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include "monitoring/CPUData.hpp"

CPUMonitorThread::CPUMonitorThread(ThreadSafeQueue<Event>& queue)
    : m_threadSafeQueue(queue), running(false) {}

CPUMonitorThread::~CPUMonitorThread() {
    stop();
}

void CPUMonitorThread::start() {
    running = true;
    m_thread = std::thread(&CPUMonitorThread::monitorLoop, this);
}

void CPUMonitorThread::stop() {
    running = false;
    if (m_thread.joinable())
        m_thread.join();
}

void CPUMonitorThread::monitorLoop() {
    CPUData data;

    while (running) {

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::ifstream statFile("/proc/stat");
        if (!statFile.is_open()) {
            std::cerr << "Failed to open /proc/stat\n";
            continue;
        }

        std::string line;
        while (std::getline(statFile, line)) {
            if (line.rfind("cpu ", 0) == 0) {  // line starts with "cpu "
                std::istringstream ss(line);
                std::string cpu_label;
                ss >> cpu_label;

                ss >> data.user >> data.nice >> data.system
                >> data.idle >> data.iowait >> data.irq >> data.softirq
                >> data.steal; // optional

                // Read extra fields if present
                uint64_t guest = 0, guest_nice = 0;
                ss >> guest >> guest_nice;
                data.user += guest;
                data.nice += guest_nice;

                break;
            }
        }

        float usage = (data.total() - data.idleTime()) * 100.0 / data.total();
        std::cout << "CPU Usage: " << usage << "%" << std::endl;

        if(usage > 0.23) {
            std::cout << "Warning: High CPU usage detected!" << std::endl;
            m_threadSafeQueue.push(Event{
                EventType::CPU_OVERLOAD,
                usage,
                std::chrono::system_clock::now()
            });
        }
        
        statFile.close();
    }
}

