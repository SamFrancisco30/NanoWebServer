#include "epoll/epoll.h"
#include "channel/channel.h"
#include "utils/utils.h"
#include <unistd.h>
#include <string.h>


#define MAX_EVENTS 1000

Epoll::Epoll() : epfd(-1), events(nullptr){
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

void Epoll::AddFd(int fd, uint32_t op){
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
}

void Epoll::updateChannel(Channel *channel){
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->getInEpoll()){
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, channel->getFd(), &ev) == -1, "epoll add error");// channel not exists in epoll, add it
        channel->setInEpoll();
    } else{
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, channel->getFd(), &ev) == -1, "epoll modify error");// channel exists in epoll, modify it
    }
}

std::vector<Channel*> Epoll::poll(int timeout){
    std::cout << "Epoll polling" << std::endl;
    std::vector<Channel*> activeChannel;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    std::cout << "nfds: " << nfds << std::endl;
    for(int i = 0; i < nfds; ++i){
        Channel *channel = static_cast<Channel*>(events[i].data.ptr);
        channel->setRevents(events[i].events);
        activeChannel.push_back(channel);
    }
    return activeChannel;
}