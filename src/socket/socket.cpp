#include "socket/socket.h"
#include "utils/utils.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

Socket::Socket(int fd) : fd_{fd} {}

Socket::Socket() : fd_{-1} {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ == -1, "socket create error");
}

Socket::~Socket() {
    if(fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

void Socket::Bind(const InetAddress& addr) {
    sockaddr_in address = addr.GetAddr();
    errif(::bind(fd_, (sockaddr*)&address, addr.GetAddrLen()) == -1, "socket bind error");
}

void Socket::Listen(int backlog) {
    errif(::listen(fd_, backlog) == -1, "socket listen error");
}

int Socket::Accept(const InetAddress& addr) {
    sockaddr_in address = addr.GetAddr();
    socklen_t addr_len = addr.GetAddrLen();
    int clnt_sockfd = accept(fd_, (sockaddr*)&address, &addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    return clnt_sockfd;
}

void Socket::SetNonBlocking() {
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::GetFd() const {
    return fd_;
}