#include "socket/inet_address.h"
#include <cstring>

InetAddress::InetAddress(const std::string& ip, uint16_t port) {
    std::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    addr_.sin_port = htons(port);
    addr_len_ = sizeof(addr_);
}

InetAddress::InetAddress() {
    std::memset(&addr_, 0, sizeof(addr_));
    addr_len_ = sizeof(addr_);
}

sockaddr_in InetAddress::GetAddr() const {
    return addr_;
}

socklen_t InetAddress::GetAddrLen() const {
    return addr_len_;
}