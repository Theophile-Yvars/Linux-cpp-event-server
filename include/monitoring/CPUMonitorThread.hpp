#pragma once

class CPUMonitorThread {
public:
    CPUMonitorThread();
    ~CPUMonitorThread();

    void start();
    void stop();
private:
    void monitorLoop();
    bool running;
};