#pragma once

#include <string>

#include "base/iprawsocket.h"

class IpRawClient : public IpRawSocket
{
public:
    IpRawClient() {}
    ~IpRawClient() {}

public:
    void setSockAddr(std::string ip, int port);
};
