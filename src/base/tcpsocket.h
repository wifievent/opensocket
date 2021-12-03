#pragma once

#include "socket.h"

class TcpSocket : public Socket
{
public:
    TcpSocket();
    ~TcpSocket() override;
    
    int send(char* buf, size_t len) override;
    int recv(char* buf, size_t len) override;
};
