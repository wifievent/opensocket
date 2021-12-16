#include <iostream>
#include <string>

#include "udpserver.h"

class UdpMsgServer : public UdpServer
{
public:
    UdpMsgServer() {}
    ~UdpMsgServer() {}

protected:
    void handleClnt() override;
};