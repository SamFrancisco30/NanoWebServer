#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/epoll.h>

class Epoll;
class Channel
{

private:
    Epoll *ep;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
public:
    Channel(Epoll *_ep, int _fd);
    ~Channel() = default;

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    void setRevents(uint32_t _ev);

    bool getInEpoll();
    void setInEpoll();

    
};

#endif