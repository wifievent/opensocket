#include "tcpserver.h"

TcpServer::TcpServer()
{
    setSockOptforReuse();
}

TcpServer::~TcpServer() {}

bool TcpServer::bind(int port)
{
    memset(&sockAddr_, 0, sizeof(sockAddr_));
    sockAddr_.sin_family = AF_INET;
    sockAddr_.sin_addr.s_addr = INADDR_ANY;
    sockAddr_.sin_port = htons(port);

    if (::bind(sock_, (struct sockaddr*)&sockAddr_, sizeof(sockAddr_)) == -1)
    {
        DLOG(ERROR) << "TcpServer::bind() failed";
        return false;
    }

    DLOG(INFO) << "TcpServer::bind() success";

    return true;
}

bool TcpServer::listen(int backlog)
{
    if (::listen(sock_, backlog) == -1)
    {
        DLOG(ERROR) << "TcpServer::listen() failed";
        return false;
    }

    DLOG(INFO) << "TcpServer::listen() success";

    return true;
}

void TcpServer::accept()
{
    DLOG(INFO) << "TcpServer::accept() open!";
    sockaddr_in cli_addr;
    unsigned int clntaddrsize = sizeof(cli_addr);
    int clntsock;

    while (true)
    { // server thread

        DLOG(INFO) << "TcpServer::accept() waiting for client";

        if ((clntsock = ::accept(sock_, (struct sockaddr*)&(cli_addr), &clntaddrsize)) < 0)
        {
            DLOG(ERROR) << "TcpServer::accept() Accept call failed";
            break;
        }

        DLOG(INFO) << "TcpServer::accept() client connected" << clntsock;

        TcpClientSocket* newsocket = new TcpClientSocket(clntsock);
        DLOG(INFO) << "TcpServer::accept() new socket created" << newsocket->sock_;
        
        newsocket->handlethread_ = new std::thread(&TcpServer::openHandleClnt, this, newsocket);

        clntsocks_.mutex_.lock();
        clntsocks_.insert(newsocket);
        clntsocks_.mutex_.unlock();
    }
}

bool TcpServer::start(int port, int backlog)
{
    if (bind(port) && listen(backlog))
    {
        DLOG(INFO) << "TcpServer::start() success";
        acceptthread_ = new std::thread(&TcpServer::accept, this);
        return true;
    }

    DLOG(ERROR) << "TcpServer::start() failed";
    return false;
}

bool TcpServer::stop()
{
    DLOG(INFO) << "TcpServer::stop() start";
    disconnect();
    DLOG(INFO) << "TcpServer::stop() acceptthread join start";
    acceptthread_->join();
    DLOG(INFO) << "TcpServer::stop() acceptthread join end";

    clntsocks_.mutex_.lock();
    for (TcpClientSocket* socket : clntsocks_)
    {
        socket->disconnect();
        socket->handlethread_->join();
    }
    clntsocks_.mutex_.unlock();

    return true;
}

void TcpServer::openHandleClnt(TcpClientSocket* clntsock)
{
    // run
    DLOG(INFO) << "TcpServer::openHandleClnt() handleClnt start" << clntsock->sock_;
    this->handleClnt(clntsock);    // join
    DLOG(INFO) << "TcpServer::openHandleClnt() handleClnt end" << clntsock->sock_;

    this->deleteClnt(clntsock);
    DLOG(INFO) << "TcpServer::openHandleClnt() Client delete" << clntsock->sock_;
}

void TcpServer::deleteClnt(TcpClientSocket* clntsock)
{
    clntsocks_.mutex_.lock();
    clntsocks_.erase(clntsock);
    clntsocks_.mutex_.unlock();

    delete clntsock;
    return;
}

int TcpServer::setSockOptforReuse()
{
    int optval = 1;
    int result = 1;

#ifdef _WIN32
    result = setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
#endif
#ifdef __linux__
    result = setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#endif

    return result; // success 0, fail -1
}
