#pragma once

#include "socket.h"

class IpRawSocket : public Socket
{
public:
    IpRawSocket();
    ~IpRawSocket() override;

    int send(char* buf, size_t len) override;
    int recv(char* buf, size_t len) override;
};
