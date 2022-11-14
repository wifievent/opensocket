#include "sslclient.h"

SslClient::SslClient() {
    ctx_ = nullptr;
    DLOG(INFO) << "SslServer::SslServer() default version";
    createContext();
}

SslClient::SslClient(double version) {
    ctx_ = nullptr;
    verison_ = version;
    DLOG(INFO) << "SslServer::SslServer() " << verison_ << " version";
    createContext();
}

int SslClient::connect(std::string ip, int port, char* cipherlist) { // connect
    if(!configureContext(cipherlist)) {
        exit(-1);
    }

    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_addr.s_addr = inet_addr(ip.c_str());
    sockAddr_.sin_port = htons(port);
    if(::connect(sock_, (struct sockaddr *)&(sockAddr_), sizeof(sockAddr_)) != 0) {
        DLOG(ERROR) << "SslClient::connect() connect error";
        DLOG(ERROR) << "SslClient::connect() Can't connect to server";
        return -1;
    }
    DLOG(INFO) << "SslClient::connect() connect success";

    ssl_ = SSL_new(ctx_);
    SSL_set_fd(ssl_, sock_);
    if(SSL_connect(ssl_) == -1) {
        DLOG(ERROR) << "SslClient::connect() SSL_connect error";
        return -1;
    }
    DLOG(INFO) << "SslClient::connect() SSL_connect success";

    return 0;
    // success 0, fail -1
    //SSL_shutdown(ssl_);
    //SSL_free(ssl_);
    //SSL_CTX_free(ctx_);
}

bool SslClient::createContext() {
    const SSL_METHOD* method;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    if(verison_ == 1.0) {
        method = TLSv1_client_method();
    }
    else if(verison_ == 1.1) {
        method = TLSv1_1_client_method();
    }
    else if(verison_ == 1.2) {
        method = TLSv1_2_client_method();
    }
    else {
        method = TLS_client_method();
    }
    
    ctx_ = SSL_CTX_new(method);
    if(!ctx_) {
        DLOG(ERROR) << "SslClient::createContext() SSL_CTX_new error";
        DLOG(ERROR) << "SslClient::createContext() Can't create SSL context";
        return false;
    }

    DLOG(INFO) << "SslClient::createContext() SSL_CTX_new success";
    return true;
}

void SslClient::freeContext() {
    if(ctx_ != nullptr) {
        SSL_CTX_free(ctx_);
    }
}

bool SslClient::configureContext(char* cipherlist) {
    if(verison_ == 0.0) {
        if(!SSL_CTX_set_ciphersuites(ctx_, cipherlist)) {
            DLOG(ERROR) << "SslServer::createContext() SSL_CTX_set_cipher_list failed";
            return false;
        }
    }
    else {
        if(!SSL_CTX_set_cipher_list(ctx_, cipherlist)) {
            DLOG(ERROR) << "SslServer::createContext() SSL_CTX_set_cipher_list failed";
            return false;
        }
    }
    return true;
}