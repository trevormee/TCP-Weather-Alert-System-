// TcpClient.hpp

#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class TcpClient
{
    private:
        int client_fd;
        std::string server_ip;
        int server_port;

    public:
        TcpClient(const std::string& ip, int port);
        ~TcpClient();
        bool connectToServer();
        int sendData(const char* data);
        std::string receiveData();
        int getClientFd();
        void closeConnection();
};
#endif