#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#endif

#ifdef __linux__
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <glog/logging.h>

#define BUFSIZE 65536

class Socket
{
public:
    int sock_{0};
    struct sockaddr_in sockAddr_;

public:
    Socket() {}
    virtual ~Socket() {}

    virtual int disconnect();
    virtual int send(char* buf, size_t len) = 0;
    virtual int recv(char* buf, size_t len) = 0;
};
