#include "channel/channel.h"
#include "server/event_loop.h"

Channel::Channel(EventLoop *_eloop, int _fd) : loop(_eloop), fd(_fd), events(0), revents(0), inEpoll(false) {}

void Channel::enableReading() {
    events |= EPOLLIN;
    loop->updateChannel(this);
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

void Channel::handleEvent() {
    callback();
}

void Channel::setCallback(std::function<void()> cb) {
    callback = cb;
}