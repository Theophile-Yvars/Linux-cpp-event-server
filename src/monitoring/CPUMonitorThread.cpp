#include "monitoring/CPUMonitorThread.hpp"
#include <thread>
#include <iostream>
#include <fstream>
#include "CPUData.hpp"
#include <sstream>
#include "queue/ThreadSafeQueue.hpp"

CPUMonitorThread::CPUMonitorThread() : running(false) {}

CPUMonitorThread::~CPUMonitorThread() {
    stop();
}

void CPUMonitorThread::start() {
    running = true;
    std::thread(&CPUMonitorThread::monitorLoop, this).detach();
}

void CPUMonitorThread::stop() {
    running = false;
}

void CPUMonitorThread::monitorLoop() {
    CPUData data;

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::fstream statFile("/proc/stat", std::ios_base::in);
        if(!statFile.is_open()) {
            std::cerr << "Failed to open /proc/stat" << std::endl;
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

        if(usage > 80.0) {
            std::cout << "Warning: High CPU usage detected!" << std::endl;
        }
        
        statFile.close();
    }
}

