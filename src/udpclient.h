#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <string>

#include "udpsocket.h"

class UdpClient : public UdpSocket
{
public:
    UdpClient() {}
    ~UdpClient() {}

public:
    int setSocketBroadcastable();
    void setSockAddr(std::string ip, int port);
    void setRecvTimeout(int sec, int millisec);
};

#endif // UDPCLIENT_H
