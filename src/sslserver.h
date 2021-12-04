#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <set>
#include <thread>
#include <mutex>

#include "base/sslsocket.h"

struct SslClientSocket : public SslSocket
{
    SslClientSocket(int sock) {sock_ = sock;}
    std::thread* handlethread_{nullptr};
};

class SocketList : public std::set<SslClientSocket*>
{
public:
    std::mutex mutex_;
};

class SslServer : public SslSocket
{
    SocketList clntsocks_;
    std::thread* acceptthread_{nullptr};

public:
    SslServer();
    ~SslServer();

public:
    bool bind(int port);
    bool listen(int backlog = 10);
    void accept();
    bool start(int port, std::string certFilePath, std::string keyFilePath, int backlog = 10);
    bool stop();

protected:
    int setSockOptforReuse();
    void deleteClnt(SslClientSocket* clntsock);
    void openHandleClnt(SslClientSocket* clntsock);
    virtual void handleClnt(SslClientSocket* clntsock) = 0;
    bool createContext();
    bool configureContext(std::string certFilePath, std::string keyFilePath);
};

#endif // SSLSERVER_H