#include "udpsocket.h"

UdpSocket::UdpSocket() {
    if((sock_ = socket(PF_INET, SOCK_DGRAM, 0) < 0)) {
        spdlog::debug("socket create failed");
    }
}

UdpSocket::~UdpSocket() {
    disconnect();
}

int UdpSocket::send(char* buf, size_t len) {
    return ::sendto(sock_, buf, len, 0, (struct sockaddr*)&sockAddr_, sizeof(sockAddr_));
}

int UdpSocket::recv(char* buf, size_t len) {
    #ifdef _WIN32
    int sockLen = sizeof(sockAddr_);
    #endif
    #ifdef __linux
    socklen_t sockLen = sizeof(sockAddr_);
    #endif

    memset(buf, 0, len);
    ssize_t recv_len = ::recvfrom(sock_, buf, len, 0, (struct sockaddr*)&sockAddr_, &sockLen);
    
    if (recv_len == -1) {
        spdlog::debug("Can't recv data");
    }

    return recv_len;
}
