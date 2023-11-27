#include "server/event_loop.h"
#include "channel/channel.h"
#include "epoll/epoll.h"


EventLoop::EventLoop() : ep(new Epoll()), quit(false) {}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop(){
    while(!quit){
    std::vector<Channel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it){
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *channel) {
    ep->updateChannel(channel);
}