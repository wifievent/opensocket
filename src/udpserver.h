#pragma once

#include <thread>

#include "base/udpsocket.h"

class UdpServer : public UdpSocket
{
public:
    UdpServer();
    ~UdpServer();

    bool bind(int port);
    bool start(int port);
    bool stop();

protected:
    std::thread* th_;

    int setSockOptforReuse();
    void openHandleClnt();
    virtual void handleClnt() = 0;

};
