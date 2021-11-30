#include "iprawclient.h"

void IpRawClient::setSockAddr(std::string ip, int port)
{
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_port = htons(port);
    sockAddr_.sin_addr.s_addr = inet_addr(ip.c_str());
    memset(&sockAddr_.sin_zero, 0, sizeof(sockAddr_.sin_zero));
}