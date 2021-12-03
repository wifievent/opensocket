#pragma once

#include "socket.h"

class UdpSocket : public Socket
{
public:
    UdpSocket();
    ~UdpSocket() override;

    int send(char* buf, size_t len) override;
    int recv(char* buf, size_t len) override;
};
