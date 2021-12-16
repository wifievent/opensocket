#include "udpclient.h"

int UdpClient::setSocketBroadcastable()
{
    int optval = 1;
    int result = 1;

#ifdef _WIN32
    result = setsockopt(sock_, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
#endif
#ifdef __linux__
    result = setsockopt(sock_, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
#endif

    return result; // success 0, fail -1
}

void UdpClient::setSockAddr(std::string ip, int port)
{
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_port = htons(port);
    sockAddr_.sin_addr.s_addr = inet_addr(ip.c_str());
    memset(&sockAddr_.sin_zero, 0, sizeof(sockAddr_.sin_zero));
}

void UdpClient::setRecvTimeout(int sec, int millisec)
{
    struct timeval optVal = {sec, millisec}; // sec, millisec
    int optLen = sizeof(optVal);

#ifdef _WIM32
    setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, (char*)&optVal, optLen);
#endif
#ifdef __linux__
    setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &optVal, optLen);
#endif
}
