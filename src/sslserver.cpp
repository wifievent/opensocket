#include "sslserver.h"

SslServer::SslServer() {
    setSockOptforReuse();
}

SslServer::~SslServer() {}

bool SslServer::bind(int port) {
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_addr.s_addr = INADDR_ANY;
    sockAddr_.sin_port = htons(port);
    
    if(::bind(sock_, (struct sockaddr*)&sockAddr_, sizeof(sockAddr_)) == -1) {
        spdlog::info("bind() error");
        return false;
    }

    return true;
}

bool SslServer::listen(int backlog) {
    if(::listen(sock_, backlog)==-1) {
        spdlog::info("listen() error");
        return false;
    }

    return true;
}

void SslServer::accept() {
    spdlog::info("open!");
    sockaddr_in cli_addr;
    unsigned int clntaddrsize = sizeof(cli_addr);
    int clntsock;

    while(true) {    // server thread
        spdlog::info("Wait for accept client");
        
        if((clntsock=::accept(sock_, (struct sockaddr*)&cli_addr, &clntaddrsize)) < 0) {
            spdlog::info("Accept call failed");
            break;
        }
        
        spdlog::info("client accept: {}", clntsock);
        
        spdlog::info("start make newsocket");
        SslClientSocket* newsocket = new SslClientSocket(clntsock);
        spdlog::info("end make newsocket");

        newsocket->ssl_ = SSL_new(ctx_);
        SSL_set_fd(newsocket->ssl_, newsocket->sock_);
        if(SSL_accept(newsocket->ssl_) <= 0) {
            spdlog::info("SSL_accept error");
            ERR_print_errors_fp(stderr);
        }

        newsocket->handlethread_ = new std::thread(&SslServer::openHandleClnt, this, newsocket);
        
        clntsocks_.mutex_.lock();
        clntsocks_.insert(newsocket);
        clntsocks_.mutex_.unlock();
    }
}

bool SslServer::start(int port, std::string certFilePath, std::string keyFilePath, int backlog) {
    createContext();
    configureContext(certFilePath, keyFilePath);

    if(bind(port) && listen(backlog)) {
        spdlog::info("bind() listen() success");
        acceptthread_ = new std::thread(&SslServer::accept, this);
        return true;
    }

    spdlog::info("bind() listen() fail");
    return false;
}

bool SslServer::stop() {
    spdlog::info("acceptthread join start");
    acceptthread_->join();
    spdlog::info("acceptthread join end");
    disconnect();

    clntsocks_.mutex_.lock();
    for(SslClientSocket* socket : clntsocks_) {
        SSL_shutdown(socket->ssl_);
        SSL_free(socket->ssl_);
        socket->disconnect();
        socket->handlethread_->join();
    }
    clntsocks_.mutex_.unlock();
    SSL_CTX_free(ctx_);
    return true;
}

void SslServer::openHandleClnt(SslClientSocket* clntsock) {    // run
    spdlog::info("start handleclnt of {}", clntsock->sock_);
    this->handleClnt(clntsock);
    spdlog::info("end handleclnt of {}", clntsock->sock_);
    //join
    spdlog::info("start deleteClnt of {}", clntsock->sock_);
    this->deleteClnt(clntsock);
    spdlog::info("end deleteClnt of {}", clntsock->sock_);
}

void SslServer::deleteClnt(SslClientSocket* clntsock) {
    clntsocks_.mutex_.lock();
    clntsocks_.erase(clntsock);
    clntsocks_.mutex_.unlock();

    delete clntsock;
    return;
}

bool SslServer::createContext() {
    const SSL_METHOD* method;

    method = TLS_server_method();

    ctx_ = SSL_CTX_new(method);
    if(!ctx_) {
        spdlog::debug("Unable to create SSL context");
        return false;
    }
    return true;
}

bool SslServer::configureContext(std::string certFilePath, std::string keyFilePath) {
    if(SSL_CTX_use_certificate_file(ctx_, certFilePath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        spdlog::debug("Unable to configureContext in certfile");
        return false;
    }
    if(SSL_CTX_use_PrivateKey_file(ctx_, keyFilePath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        spdlog::debug("Unable to configureContext in keyfile");
        return false;
    }
    return true;
}

int SslServer::setSockOptforReuse() {
    int optval = 1;
    int result = 1;

    #ifdef _WIN32
    result = setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
    #endif
    #ifdef __linux__
    result = setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    #endif

    return result; //success 0, fail -1
}
