#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>

#include "base/tcpsocket.h"

class TcpClient : public TcpSocket //tcp
{
public:
    //how about get TcpSocket* tcpsock_ for arg
    TcpClient() {}
    ~TcpClient() {}

public:
    int connect(std::string ip, int port);
};

#endif // TCPCLIENT_H
