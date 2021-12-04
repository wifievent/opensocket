#pragma once

#include <set>
#include <thread>
#include <mutex>

#include "base/tcpsocket.h"

struct TcpClientSocket : public TcpSocket
{
    TcpClientSocket(int sock) { sock_ = sock; }
    std::thread* handlethread_{nullptr};
};

class SocketList : public std::set<TcpClientSocket*>
{
public:
    std::mutex mutex_;
};

class TcpServer : public TcpSocket
{
    SocketList clntsocks_;
    std::thread* acceptthread_{nullptr};

public:
    TcpServer();
    ~TcpServer();

public:
    bool bind(int port);
    bool listen(int backlog = 10);
    void accept();
    bool start(int port, int backlog = 10);
    bool stop();

protected:
    int setSockOptforReuse();
    void deleteClnt(TcpClientSocket* clntsock);
    void openHandleClnt(TcpClientSocket* clntsock);
    virtual void handleClnt(TcpClientSocket* clntsock) = 0;
};
