#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel
{
private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;

public:
    std::function<void()> callback;

    Channel(EventLoop *_eloop, int _fd);
    ~Channel() = default;

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    void setRevents(uint32_t _ev);

    bool getInEpoll();
    void setInEpoll();

    void handleEvent();
    void setCallback(std::function<void()> cb);
};

#endif