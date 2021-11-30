#pragma once

#include <thread>

#include "base/iprawsocket.h"

class IpRawServer : public IpRawSocket
{
public:
    IpRawServer();
    ~IpRawServer();

    bool bind(int port);
    bool start(int port);
    bool stop();

protected:
    std::thread* th_;

    int setSockOptforReuse();
    void openHandleClnt();
    virtual void handleClnt() = 0;

};
