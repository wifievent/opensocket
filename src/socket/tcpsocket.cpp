#include "tcpsocket.h"

TcpSocket::TcpSocket() {
    if ((sock_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        spdlog::debug("socket create failed");
    }
}

TcpSocket::~TcpSocket() {
    disconnect();
}

int TcpSocket::send(char* buf, size_t len) {
    return ::send(sock_, buf, len, 0);
    // success 0, fail -1
}

int TcpSocket::recv(char* buf, size_t len) {
    memset(buf, 0, len);
    ssize_t recv_len = ::recv(sock_, buf, len, 0);

    if (recv_len == -1) {
        spdlog::debug("Can't recv data");
    }

    return recv_len;
}
