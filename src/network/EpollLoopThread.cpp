#include "network/EpollLoopThread.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <iostream>

#include "mock/MessageHeader.hpp"
#include "event/Event.hpp"

#define SOCKET_PATH "/tmp/engine.sock"

EpollLoopThread::EpollLoopThread(ThreadSafeQueue<Event>& queue)
    : _running(false),
      _queue(queue),
      _epollFd(-1),
      _serverFd(-1)
{
}

EpollLoopThread::~EpollLoopThread()
{
    stop();
}

void EpollLoopThread::start()
{
    _running = true;
    _thread = std::thread(&EpollLoopThread::_epollLoop, this);
}

void EpollLoopThread::stop()
{
    _running = false;

    if (_thread.joinable())
        _thread.join();
}

void EpollLoopThread::_epollLoop()
{
    // Supprimer ancien socket si existe
    unlink(SOCKET_PATH);

    // epoll
    _epollFd = epoll_create1(0);
    if (_epollFd == -1) {
        perror("epoll_create1");
        return;
    }

    // socket UNIX
    _serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (_serverFd == -1) {
        perror("socket");
        return;
    }

    // Non bloquant
    int flags = fcntl(_serverFd, F_GETFL, 0);
    fcntl(_serverFd, F_SETFL, flags | O_NONBLOCK);

    //Bind
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(_serverFd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return;
    }

    // Listen
    if (listen(_serverFd, SOMAXCONN) < 0) {
        perror("listen");
        return;
    }

    // Ajouter à epoll
    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = _serverFd;

    epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &event);

    constexpr int MAX_EVENTS = 16;
    epoll_event events[MAX_EVENTS];

    std::cout << "[Engine] Listening on " << SOCKET_PATH << "\n";

    while (_running)
    {
        int nfds = epoll_wait(_epollFd, events, MAX_EVENTS, 1000);

        if (nfds <= 0)
            continue;

        for (int i = 0; i < nfds; i++)
        {
            int fd = events[i].data.fd;

            // Nouvelle connexion
            if (fd == _serverFd)
            {
                while (true)
                {
                    int clientFd = accept(_serverFd, nullptr, nullptr);

                    if (clientFd == -1)
                        break;

                    int cflags = fcntl(clientFd, F_GETFL, 0);
                    fcntl(clientFd, F_SETFL, cflags | O_NONBLOCK);

                    epoll_event clientEvent{};
                    clientEvent.events = EPOLLIN;
                    clientEvent.data.fd = clientFd;

                    epoll_ctl(_epollFd,
                              EPOLL_CTL_ADD,
                              clientFd,
                              &clientEvent);

                    std::cout << "[Engine] Client connected\n";
                }
            }

            // Données client
            else
            {
                //Lire header
                MessageHeader header;
                ssize_t bytes = recv(fd, &header, sizeof(header), 0);

                if (bytes <= 0)
                {
                    close(fd);
                    epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr);
                    std::cout << "[Engine] Client disconnected\n";
                    continue;
                }

                // Lire payload
                if (header.size != sizeof(Event))
                {
                    std::cerr << "Invalid event size\n";
                    continue;
                }

                Event event;
                ssize_t payloadBytes =
                    recv(fd, &event, sizeof(Event), 0);

                if (payloadBytes <= 0)
                {
                    close(fd);
                    epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr);
                    continue;
                }

                // Push vers queue
                _queue.push(event);
            }
        }
    }

    close(_serverFd);
    close(_epollFd);
    unlink(SOCKET_PATH);

    std::cout << "[Engine] Stopped\n";
}