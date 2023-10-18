#include "channel/channel.h"
#include "epoll/epoll.h"

Channel::Channel(Epoll *_ep, int _fd) : ep(_ep), fd(_fd), events(0), revents(0), inEpoll(false) {}

void Channel::enableReading() {
    events |= EPOLLIN;
    ep->updateChannel(this);
}

int Channel::getFd() {
    return fd;
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getRevents() {
    return revents;
}

void Channel::setRevents(uint32_t _ev) {
    revents = _ev;
}

bool Channel::getInEpoll() {
    return inEpoll;
}

void Channel::setInEpoll() {
    inEpoll = true;
}