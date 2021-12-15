#include <iostream>
#include <string>
#include <thread>

#include "udpclient.h"

class UdpMsgClient : public UdpClient
{
    char sendbuf_[BUFSIZE];

public:
    UdpMsgClient() {}
    ~UdpMsgClient() {}

    void start(std::string ip, int port);
    void stop();
    void send_message(std::string message);
};