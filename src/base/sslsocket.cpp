#include "sslsocket.h"

SslSocket::SslSocket() {
    if ((sock_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        spdlog::info("socket create failed");
    }
    spdlog::info("socket create success");
}

SslSocket::~SslSocket() {
    disconnect();
}

bool SslSocket::createContext() {
    const SSL_METHOD* method;

    method = TLS_server_method();

    ctx_ = SSL_CTX_new(method);
    if(!ctx_) {
        spdlog::debug("Unable to create SSL context");
        return false;
    }
    return true;
}

bool SslSocket::configureContext(std::string certFilePath, std::string keyFilePath) {
    //../../
    if(SSL_CTX_use_certificate_file(ctx_, ("../../"+certFilePath).c_str(), SSL_FILETYPE_PEM) <= 0) {
        spdlog::debug("Unable to configureContext in certfile");
        return false;
    }
    if(SSL_CTX_use_PrivateKey_file(ctx_, ("../../"+keyFilePath).c_str(), SSL_FILETYPE_PEM) <= 0) {
        spdlog::debug("Unable to configureContext in keyfile");
        return false;
    }
    return true;
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