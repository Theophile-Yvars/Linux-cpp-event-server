#include "mock/EventProduction.hpp"
#include "event/Event.hpp"
#include "event/EventType.hpp"
#include "mock/MessageHeader.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <cstdlib>
#include <random>

EventProduction::EventProduction(EventType eventType) : _running(false), _eventType(eventType) {
}

EventProduction::~EventProduction() {
    stop();
}

void EventProduction::start() {
    _running = true;
    _thread = std::thread(&EventProduction::produceEvent, this);
}

void EventProduction::stop() {
    _running = false;
    if (_thread.joinable()) {
        _thread.join();
    }
}

void EventProduction::produceEvent() {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/engine.sock", sizeof(addr.sun_path)-1);

    while (_running && connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 500); // millisecondes

    while (_running) {
        Event e{
            _eventType, 
            static_cast<uint32_t>(rand() % 100), 
            std::chrono::system_clock::now()
        };

        MessageHeader header;
        header.size = sizeof(Event);
        
        if (send(sock, &header, sizeof(header), 0) < 0) break;
        if (send(sock, &e, sizeof(e), 0) < 0) break;

        int waitMs = dis(gen);
        std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
    }
    
    if (sock != -1) {
        close(sock);
    }
}