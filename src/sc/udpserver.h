#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <thread>

#include "udpsocket.h"

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

#endif // UDPSERVER_H
