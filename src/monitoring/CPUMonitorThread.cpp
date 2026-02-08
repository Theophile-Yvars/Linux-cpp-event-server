#include "monitoring/CPUMonitorThread.hpp"
#include <thread>
#include <iostream>
#include <fstream>

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
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::fstream MyFile("/proc/stat", std::ios_base::in);
        std::string myText;
        getline(MyFile, myText);
        std::cout << myText << std::endl;
        MyFile.close();
    }
}

