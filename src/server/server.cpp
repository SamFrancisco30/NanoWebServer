#include <iostream>
#include <cerrno>
#include <memory>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include "utils/utils.h"
#include "socket/socket.h"
#include "socket/inet_address.h"
#include "epoll/epoll.h"
#include "channel/channel.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void handleReadEvent(int sockfd){
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

int main() {
    auto socket = std::make_unique<Socket>();

    InetAddress addr("127.0.0.1", 8080);

    socket->Bind(addr);

    socket->Listen();
    
    auto epoll = std::make_unique<Epoll>();

    Channel *server_channel = new Channel(epoll.get(), socket->GetFd());

    server_channel->enableReading();

    while (true) {
        std::vector<Channel*> active_channels = epoll->poll();
        int nfds = active_channels.size();
        for(int i = 0; i < nfds; ++i){
            int fd = active_channels[i]->getFd();
            if(fd == socket->GetFd()){       // new client connection
                InetAddress clnt_addr;  
                Socket *clnt_sock = new Socket(socket->Accept(clnt_addr));  
                printf("new client fd %d!\n", clnt_sock->GetFd());
                clnt_sock->SetNonBlocking();
                Channel *clnt_channel = new Channel(epoll.get(), clnt_sock->GetFd());
                clnt_channel->enableReading();
            } else if(active_channels[i]->getRevents() & EPOLLIN){      // read event
                handleReadEvent(fd);
            } else{         
                printf("something else happened\n");
            }
        }
    }
    return 0;
}
