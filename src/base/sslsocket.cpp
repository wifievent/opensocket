#include "sslsocket.h"

SslSocket::SslSocket() {
    if ((sock_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        spdlog::info("socket create failed");
    }
    spdlog::info("socket create success");
}

SslSocket::~SslSocket() {
    disconnect();
    //SSL_CTX_free(ctx_);
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
        spdlog::info("Can't recv data");
    }

    return recv_len;
}
