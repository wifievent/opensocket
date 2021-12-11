#include "sslsocket.h"

SslSocket::SslSocket() {
    if ((sock_ = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        DLOG(ERROR) << "SslSocket::SslSocket() socket() failed";
        sock_ = 0;
    }
    DLOG(INFO) << "SslSocket::SslSocket() socket() success";
}

SslSocket::~SslSocket() {
    SSL_CTX_free(ctx_);
    disconnect();
}

int SslSocket::send(char* buf, size_t len)
{
    return ::SSL_write(ssl_, buf, len);
}

int SslSocket::recv(char* buf, size_t len)
{
    memset(buf, 0, len);
    ssize_t recv_len = ::SSL_read(ssl_, buf, len);

    if (recv_len == -1) {
        DLOG(INFO) << "SslSocket::recv() SSL_read() failed";
    }

    return recv_len;
}