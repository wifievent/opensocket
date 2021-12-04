#include "socket.h"

int Socket::disconnect()
{
    int result = 0;

    if (sock_ != 0)
    {
#ifdef _WIN32
        ::shutdown(sock_, SD_BOTH);
        result = ::closesocket(sock_);
#endif
#ifdef __linux__
        ::shutdown(sock_, SHUT_RDWR);
        result = ::close(sock_);
#endif

        sock_ = 0;
    }

    return result;
    // success 0, fail -1
}