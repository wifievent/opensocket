#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <set>
#include <thread>
#include <mutex>
#include <fstream>

#include "base/sslsocket.h"

struct SslClientSocket : public SslSocket
{
    SslClientSocket(int sock) {sock_ = sock;}
    std::thread* handlethread_{nullptr};
};

class SslSocketList : public std::set<SslClientSocket*>
{
public:
    std::mutex mutex_;
};

class SslServer : public SslSocket
{
    SslSocketList clntsocks_;
    std::thread* acceptthread_{nullptr};

public:
    SSL_CTX* ctx_;
    double verison_{0.0};

public:
    SslServer();
    SslServer(double version);
    ~SslServer();

public:
    bool bind(int port);
    bool listen(int backlog = 10);
    void accept();
    bool start(int port, std::string certFilePath, std::string keyFilePath, char* cipherlist, bool keylog = false, int backlog = 10);
    bool stop();

protected:
    int setSockOptforReuse();
    void deleteClnt(SslClientSocket* clntsock);
    void openHandleClnt(SslClientSocket* clntsock);
    virtual void handleClnt(SslClientSocket* clntsock) = 0;
    bool createContext();
    void freeContext();
    bool configureContext(std::string certFilePath, std::string keyFilePath, char* cipherlist);
    void keyLog();
};

#endif // SSLSERVER_H