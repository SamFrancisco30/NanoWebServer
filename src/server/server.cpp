#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cerrno>
#include "utils/utils.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

    int epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");

    epoll_event events[MAX_EVENTS], ev;
    std::memset(&events, 0, sizeof(events));
    std::memset(&ev, 0, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    setnonblocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error");
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd) {
                sockaddr_in clnt_addr;
                std::memset(&clnt_addr, 0, sizeof(clnt_addr));
                socklen_t clnt_addr_len = sizeof(clnt_addr);

                int clnt_sockfd = accept(sockfd, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_len);
                errif(clnt_sockfd == -1, "socket accept error");
                std::cout << "New client fd " << clnt_sockfd << "! IP: " << inet_ntoa(clnt_addr.sin_addr) << " Port: " << ntohs(clnt_addr.sin_port) << std::endl;

                std::memset(&ev, 0, sizeof(ev));
                ev.data.fd = clnt_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(clnt_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);
            } else if (events[i].events & EPOLLIN) {
                char buf[READ_BUFFER];
                while (true) {
                    std::memset(&buf, 0, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if (bytes_read > 0) {
                        std::cout << "Message from client fd " << events[i].data.fd << ": " << buf << std::endl;
                        write(events[i].data.fd, buf, sizeof(buf));
                    } else if (bytes_read == -1 && errno == EINTR) {
                        std::cout << "Continue reading" << std::endl;
                        continue;
                    } else if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                        std::cout << "Finish reading once, errno: " << errno << std::endl;
                        break;
                    } else if (bytes_read == 0) {
                        std::cout << "EOF, client fd " << events[i].data.fd << " disconnected" << std::endl;
                        close(events[i].data.fd);
                        break;
                    }
                }
            } else {
                std::cout << "Something else happened" << std::endl;
            }
        }
    }
    close(sockfd);
    return 0;
}
