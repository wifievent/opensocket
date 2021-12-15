#include "udpmsgserver.h"

void UdpMsgServer::handleClnt()
{
    while(true)
    {
        char buf[BUFSIZE];
        int res = recv(buf, BUFSIZE);
        if(res > 0)
        {
            std::cout << "Received: " << buf << std::endl;
            memset(buf, 0, BUFSIZE);
        }
        else
        {
            DLOG(ERROR) << "Error receiving message";
            break;
        }
    }
}

int main()
{
    UdpMsgServer server;
    server.start(5555);
    sleep(60);
    server.stop();
}