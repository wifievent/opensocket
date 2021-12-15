#ifndef SSLSOCKET_H
#define SSLSOCKET_H

#include "socket.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

//problem to make SSL_free and SSL_shutdown

class SslSocket : public Socket
{
public:
    SSL* ssl_;

public:
    SslSocket();
    ~SslSocket() override;

public:
    int disconnect() override;
    int send(char* buf, size_t len) override;
    int recv(char* buf, size_t len) override;
};

#endif // SSLSOCKET_H
