#include <iostream>
#include <thread>
#include <chrono>
#include "monitoring/CPUMonitorThread.hpp"

using namespace std;

int main( int argc, char* argv[]) {
    cout << "Hello, World!" << endl;
    CPUMonitorThread monitor;
    monitor.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    monitor.stop();    
    return EXIT_SUCCESS;
}