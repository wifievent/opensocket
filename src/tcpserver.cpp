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
        spdlog::info("bind() error");
        return false;
    }

    return true;
}

bool TcpServer::listen(int backlog)
{
    if (::listen(sock_, backlog) == -1)
    {
        spdlog::info("listen() error");
        return false;
    }

    return true;
}

void TcpServer::accept()
{
    spdlog::info("open!");
    sockaddr_in cli_addr;
    unsigned int clntaddrsize = sizeof(cli_addr);
    int clntsock;

    while (true)
    { // server thread
        spdlog::info("Wait for accept client");

        if ((clntsock = ::accept(sock_, (struct sockaddr*)&(cli_addr), &clntaddrsize)) < 0)
        {
            spdlog::info("Accept call failed");
            break;
        }

        spdlog::info("client accept: %d", clntsock);

        TcpClientSocket* newsocket = new TcpClientSocket(clntsock);
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
        spdlog::info("bind() listen() success");
        acceptthread_ = new std::thread(&TcpServer::accept, this);
        return true;
    }

    spdlog::info("bind() listen() fail");
    return false;
}

bool TcpServer::stop()
{
    disconnect();
    acceptthread_->join();

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
{ // run
    this->handleClnt(clntsock);
    // join
    this->deleteClnt(clntsock);
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
