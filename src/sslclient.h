#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <string>

#include "base/sslsocket.h"

class SslClient : public SslSocket //ssl
{
public:
    SslClient();
    ~SslClient();

public:
    int connect(std::string ip, int port);

protected:
    bool createContext();
};

#endif // SSLCLIENT_H
