#include "tcpsocket.h"

TcpSocket::TcpSocket()
{
    if ((sock_ = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        DLOG(ERROR) << "TcpSocket::TcpSocket() socket() failed";
        sock_ = 0;
    }
    DLOG(INFO) << "TcpSocket::TcpSocket() socket() success";
}

TcpSocket::~TcpSocket()
{
    disconnect();
}

int TcpSocket::send(char* buf, size_t len)
{
    return ::send(sock_, buf, len, 0);
    // success 0, fail -1
}

int TcpSocket::recv(char* buf, size_t len)
{
    memset(buf, 0, len);
    ssize_t recv_len = ::recv(sock_, buf, len, 0);

    if (recv_len == -1)
    {
        DLOG(INFO) << "TcpSocket::recv() recv() failed";
    }

    return recv_len;
}
