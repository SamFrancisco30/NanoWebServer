#ifndef SOCKET_H
#define SOCKET_H

#include "socket/inet_address.h"

class Socket {
public:
    Socket(int fd);
    Socket();
    ~Socket();

    /// @brief bind socket to a specific address
    /// @param addr the address to bind
    void Bind(const InetAddress& addr);
    
    /// @brief listen on the socket
    /// @param backlog the maximum length of the queue of pending connections
    void Listen(int backlog = SOMAXCONN);

    /// @brief accept a connection on the socket
    /// @return the file descriptor of the accepted socket
    int Accept(const InetAddress& addr);

    /// @brief set the socket to be non-blocking
    void SetNonBlocking();

    /// @brief get the file descriptor of the socket
    /// @return the file descriptor of the socket
    int GetFd() const;

private:
    int fd_;
};

#endif