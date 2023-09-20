#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H

#include <arpa/inet.h>
#include <string>

class InetAddress {
public:
    InetAddress(const std::string& ip, uint16_t port);
    InetAddress();
    ~InetAddress() = default;

    sockaddr_in GetAddr() const;
    socklen_t GetAddrLen() const;

private:
    sockaddr_in addr_;
    socklen_t addr_len_;

};


#endif