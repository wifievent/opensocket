#include "iprawsocket.h"

IpRawSocket::IpRawSocket() {
    if((sock_ = socket(PF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        spdlog::info("socket create failed");
        sock_ = 0;
    }
    spdlog::info("socket create success");
}

IpRawSocket::~IpRawSocket() {
    disconnect();
}

int IpRawSocket::send(char* buf, size_t len) {
    return ::sendto(sock_, buf, len, 0, (struct sockaddr*)&sockAddr_, sizeof(sockAddr_));
}

int IpRawSocket::recv(char* buf, size_t len) {
#ifdef _WIN32
    int sockLen = sizeof(sockAddr_);
#endif
#ifdef __linux
    socklen_t sockLen = sizeof(sockAddr_);
#endif

    memset(buf, 0, len);
    ssize_t recv_len = ::recvfrom(sock_, buf, len, 0, (struct sockaddr*)&sockAddr_, &sockLen);
    
    if (recv_len == -1) {
        spdlog::info("Can't recv data");
    }

    return recv_len;
}

void IpRawSocket::setSockAddr(std::string ip, int port)
{
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_port = htons(port);
    sockAddr_.sin_addr.s_addr = inet_addr(ip.data());
    memset(&sockAddr_.sin_zero, 0, sizeof(sockAddr_.sin_zero));
}
