#include "network/EpollLoopThread.hpp"

EpollLoopThread::EpollLoopThread(ThreadSafeQueue<Event> &queue) : _running(false), _queue(queue) {
};

EpollLoopThread::~EpollLoopThread()
{
    stop();
};

void EpollLoopThread::start() {
    _running = true;
    _thread = std::thread(&EpollLoopThread::_epollLoop, this);
};

void EpollLoopThread::stop() {
    _running = false;
    if(_thread.joinable()){
        _thread.join();
    }
};

void EpollLoopThread::_epollLoop() {
    while(_running){
        /*
        A. L'attente (epoll_wait)
        Le thread appelle epoll_wait(). À ce moment-là, le thread est mis en pause par Linux. Il ne consomme 0% de CPU. Il attend qu'un signal arrive sur l'un des sockets surveillés (ton Mock par exemple).
        */





        /*
        B. La détection (Le réveil)
        Dès que ton Mock fait un send(), le noyau Linux réveille le thread. epoll_wait renvoie alors une liste de tous les sockets qui ont reçu des données.
        */
        




        /*
        C. Le Dispatch (La distribution)
        Le thread parcourt cette liste :

        Si c'est le Listening Socket (ton /tmp/engine.sock), il appelle accept() pour accueillir un nouveau client.

        Si c'est un Client Socket (ton Mock), il lit les données (recv), reconstruit l'objet Event, et le pousse dans la ThreadSafeQueue.
        */




        
    }
};
