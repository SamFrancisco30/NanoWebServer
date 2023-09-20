#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "utils/utils.h"

#define BUFFER_SIZE 1024

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(connect(sockfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1, "socket connect error");

    while (true) {
        char buf[BUFFER_SIZE];
        std::memset(&buf, 0, sizeof(buf));
        std::cin >> buf;
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if (write_bytes == -1) {
            std::cout << "Socket already disconnected, can't write any more!" << std::endl;
            break;
        }
        std::memset(&buf, 0, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0) {
            std::cout << "Message from server: " << buf << std::endl;
        } else if (read_bytes == 0) {
            std::cout << "Server socket disconnected!" << std::endl;
            break;
        } else if (read_bytes == -1) {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}
