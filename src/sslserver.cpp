#include "sslserver.h"

SslServer::SslServer() {
    setSockOptforReuse();
    ctx_ = nullptr;
    DLOG(INFO) << "SslServer::SslServer() default version";
    createContext();
}

SslServer::SslServer(double version) {
    setSockOptforReuse();
    ctx_ = nullptr;
    verison_ = version;
    DLOG(INFO) << "SslServer::SslServer() " << verison_ << " version";
    createContext();
}

SslServer::~SslServer() {}

bool SslServer::bind(int port) {
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_addr.s_addr = INADDR_ANY;
    sockAddr_.sin_port = htons(port);
    
    if(::bind(sock_, (struct sockaddr*)&sockAddr_, sizeof(sockAddr_)) == -1) {
        DLOG(ERROR) << "SslServer::bind() failed";
        return false;
    }
    DLOG(INFO) << "SslServer::bind() success";

    return true;
}

bool SslServer::listen(int backlog) {
    if(::listen(sock_, backlog)==-1) {
        DLOG(ERROR) << "SslServer::listen() failed";
        return false;
    }
    DLOG(INFO) << "SslServer::listen() success";

    return true;
}

void SslServer::accept() {
    DLOG(INFO) << "SslServer::accept() open!";
    sockaddr_in cli_addr;
    unsigned int clntaddrsize = sizeof(cli_addr);
    int clntsock;

    while(true) {    // server thread
        DLOG(INFO) << "SslServer::accept() waiting for client";
        
        if((clntsock=::accept(sock_, (struct sockaddr*)&cli_addr, &clntaddrsize)) < 0) {
            DLOG(ERROR) << "SslServer::accept() Accept call failed";
            break;
        }

        DLOG(INFO) << "SslServer::accept() client connected " << clntsock;
        
        SslClientSocket* newsocket = new SslClientSocket(clntsock);
        DLOG(INFO) << "SslServer::accept() new socket created " << newsocket->sock_;

        newsocket->ssl_ = SSL_new(ctx_);
        SSL_set_fd(newsocket->ssl_, newsocket->sock_);
        if(SSL_accept(newsocket->ssl_) <= 0) {
            DLOG(ERROR) << "SslServer::accept() SSL_accept failed";
            ERR_print_errors_fp(stderr);
        }

        DLOG(INFO) << "SslServer::accept() SSL_accept success";

        newsocket->handlethread_ = new std::thread(&SslServer::openHandleClnt, this, newsocket);
        
        clntsocks_.mutex_.lock();
        clntsocks_.insert(newsocket);
        clntsocks_.mutex_.unlock();
    }
}

bool SslServer::start(int port, std::string certFilePath, std::string keyFilePath, char* cipherlist, bool keylog, int backlog) {
    if (keylog) {
        keyLog();
    }
    
    if(!configureContext(certFilePath, keyFilePath, cipherlist)) {
        exit(-1);
    }

    if(bind(port) && listen(backlog)) {
        DLOG(INFO) << "SslServer::start() success";
        acceptthread_ = new std::thread(&SslServer::accept, this);
        return true;
    }

    DLOG(ERROR) << "SslServer::start() failed";
    return false;
}

bool SslServer::stop() {
    DLOG(INFO) << "SslServer::stop() start";
    disconnect();
    DLOG(INFO) << "SslServer::stop() acceptthread join start";
    acceptthread_->join();
    DLOG(INFO) << "SslServer::stop() acceptthread join end";

    clntsocks_.mutex_.lock();
    for(SslClientSocket* socket : clntsocks_) {
        socket->disconnect();
        socket->handlethread_->join();
    }
    clntsocks_.mutex_.unlock();
    return true;
}

void SslServer::openHandleClnt(SslClientSocket* clntsock) {    // run
    DLOG(INFO) << "SslServer::openHandleClnt() handleClnt start " << clntsock->sock_;
    this->handleClnt(clntsock);    // join
    DLOG(INFO) << "SslServer::openHandleClnt() handleClnt end " << clntsock->sock_;

    this->deleteClnt(clntsock);
    DLOG(INFO) << "SslServer::openHandleClnt() Client delete " << clntsock->sock_;
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

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    if(verison_ == 1.0) {
        method = TLSv1_server_method();
    }
    else if(verison_ == 1.1) {
        method = TLSv1_1_server_method();
    }
    else if(verison_ == 1.2) {
        method = TLSv1_2_server_method();
    }
    else {
        method = TLS_server_method();
    }

    ctx_ = SSL_CTX_new(method);
    if(!ctx_) {
        DLOG(ERROR) << "SslServer::createContext() SSL_CTX_new failed";
        DLOG(ERROR) << "SslServer::createContext() Unable to create SSL context";
        return false;
    }

    DLOG(INFO) << "SslServer::createContext() SSL_CTX_new success";
    return true;
}

void SslServer::freeContext() {
    if(ctx_ != nullptr) {
        SSL_CTX_free(ctx_);
    }
}

bool SslServer::configureContext(std::string certFilePath, std::string keyFilePath, char* cipherlist) {
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

    if(SSL_CTX_use_certificate_file(ctx_, certFilePath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        DLOG(ERROR) << "SslServer::configureContext() SSL_CTX_use_certificate_file failed";
        DLOG(ERROR) << "SslServer::configureContext() Unable to configure context in certificate";
        return false;
    }
    if(SSL_CTX_use_PrivateKey_file(ctx_, keyFilePath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        DLOG(ERROR) << "SslServer::configureContext() SSL_CTX_use_PrivateKey_file failed";
        DLOG(ERROR) << "SslServer::configureContext() Unable to configure context in key";
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

void SslServer::keyLog() {
    SSL_CTX_set_keylog_callback(
        this->ctx_,
        [](SSL const*, char const* line) {
            FILE  * fp;
            fp = fopen("key_log.log", "a");
            if (fp == NULL) {
                DLOG(INFO) << "Failed to create log file";
            }
            fprintf(fp, "%s\n", line);
            fclose(fp);
            DLOG(INFO) << line;
        }
    );
}