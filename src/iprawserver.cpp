#include "iprawserver.h"

IpRawServer::IpRawServer()
{
    setSockOptforReuse();
}

IpRawServer::~IpRawServer() {}

bool IpRawServer::bind(int port)
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
bool IpRawServer::start(int port)
{
    if (bind(port))
    {
        spdlog::info("bind() success");
        th_ = new std::thread(&IpRawServer::openHandleClnt, this);
        return true;
    }
    spdlog::info("bind() fail");
    return false;
}
bool IpRawServer::stop()
{
    disconnect();
    th_->join();
    return true;
}

int IpRawServer::setSockOptforReuse()
{
    int optval = 1;
    int result = 1;

#ifdef _WIN32
    result = setsockopt(sock_, IPPROTO_IP, IP_HDRINCL, (char*)&optval, sizeof(optval));
#endif
#ifdef __linux__
    result = setsockopt(sock_, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval));
#endif

    return result; // success 0, fail -1
}

void IpRawServer::openHandleClnt()
{
    handleClnt();
}
