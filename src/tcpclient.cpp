#include "tcpclient.h"

int TcpClient::connect(std::string ip, int port) // connect
{
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_addr.s_addr = inet_addr(ip.c_str());
    sockAddr_.sin_port = htons(port);
    return ::connect(sock_, (struct sockaddr*)&(sockAddr_), sizeof(sockAddr_));
    // success 0, fail -1
}
