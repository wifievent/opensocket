#pragma once

#include <string>

#include "base/udpsocket.h"

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
