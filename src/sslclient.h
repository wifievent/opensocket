#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <string>

#include "base/sslsocket.h"

class SslClient : public SslSocket //ssl
{
public:
    SSL_CTX* ctx_;
    float verison_{0.0};

public:
    SslClient();
    SslClient(float version);
    ~SslClient() {};

public:
    int connect(std::string ip, int port, char* cipherlist);

protected:
    bool createContext();
    void freeContext();
    bool configureContext(char* cipherlist);
};

#endif // SSLCLIENT_H
