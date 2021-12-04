#include "sslclient.h"

SslClient::SslClient() {
}

int SslClient::connect(std::string ip, int port) { // connect
    createContext();
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_addr.s_addr = inet_addr(ip.c_str());
    sockAddr_.sin_port = htons(port);
    if(::connect(sock_, (struct sockaddr *)&(sockAddr_), sizeof(sockAddr_)) != 0) {
        spdlog::debug("Cannot ::connect to server");
        return -1;
    }
    ssl_ = SSL_new(ctx_);
    SSL_set_fd(ssl_, sock_);
    if(SSL_connect(ssl_) == -1) {
        spdlog::debug("SSL_connect error");
        return -1;
    }
    return 0;
    // success 0, fail -1
    //SSL_shutdown(ssl_);
    //SSL_free(ssl_);
    //SSL_CTX_free(ctx_);
}

bool SslClient::createContext() {
    const SSL_METHOD* method;

    method = TLS_client_method();

    ctx_ = SSL_CTX_new(method);
    if(!ctx_) {
        spdlog::debug("Unable to create SSL context");
        return false;
    }
    return true;
}