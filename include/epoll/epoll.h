#ifndef EPOLL_H
#define EPOLL_H
#include <sys/epoll.h>
#include <vector>
class Channel;
class Epoll
{
private:
    int epfd;
    struct epoll_event *events;
public:
    Epoll();
    ~Epoll();

    void AddFd(int fd, uint32_t op);
    void updateChannel(Channel *channel);
    std::vector<Channel*> poll(int timeout = -1);
};


#endif