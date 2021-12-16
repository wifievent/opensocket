#include "udpmsgclient.h"

void UdpMsgClient::start(std::string ip, int port)
{
    setSocketBroadcastable();
    setSockAddr(ip, port);
}

void UdpMsgClient::stop()
{
    disconnect();
}

void UdpMsgClient::send_message(std::string message)
{
    strncpy(sendbuf_, message.c_str(), message.length());
    int result = send(sendbuf_, strlen(sendbuf_) + 1);
    if(result == -1)
    {
        DLOG(ERROR) << "Error sending message";
    }
    memset(sendbuf_, 0, BUFSIZE);
}

int main()
{
    UdpMsgClient messenger;

    messenger.start("192.168.25.63", 5555);

    while(true)
    {
        std::string message;
        std::cout << "Enter message: ";
        getline(std::cin, message);
        messenger.send_message(message);
        
        if(message.compare("exit") == 0)
        {
            break;
        }
    }

    messenger.stop();
}