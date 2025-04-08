/***************************************************************
  Student Name: Trevor Mee
  File Name: TcpClient.hpp
  Project 3 

  @brief Contains the function declarations for TcpClient class
         of the weather alert system. This class defines the 
         various functions related to the client side of the 
         client/server tcp paradigm including connecting to 
         a server, reading from a server, and writing to a server
***************************************************************/

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
        // private instance variables
        int client_fd;
        std::string server_ip;
        int server_port;

    public:
        // function declarations
        TcpClient(const std::string& ip, int port);
        ~TcpClient();
        bool connectToServer();
        int sendData(const char* data);
        std::string receiveData();
        int getClientFd();
        void closeConnection();
};
#endif