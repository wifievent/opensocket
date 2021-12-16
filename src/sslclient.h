#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <string>

#include "base/sslsocket.h"

class SslClient : public SslSocket //ssl
{
public:
    SSL_CTX* ctx_;

public:
    SslClient();
    ~SslClient() {};

public:
    int connect(std::string ip, int port);

protected:
    bool createContext();
    void freeContext();
};

#endif // SSLCLIENT_H
