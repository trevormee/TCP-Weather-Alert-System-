// TcpServer.hpp
#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include "User.hpp"
#include "Authentication.hpp"

#include <stdio.h>          
#include <stdlib.h>         
#include <sys/socket.h>     
#include <sys/types.h>      
#include <netinet/in.h>    
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <csignal>
#include <cstring>
#include <pthread.h>

class TcpServer
{
    private:
        int server_fd;
        int port;
        const int NUM_CONCURRENT_CONNECTIONS = 10;

    public:
        TcpServer(int port);
        ~TcpServer();
        bool startServer();
        void runServer();
        static void* handleClient(void* arg);
        int getServerFd() const { return server_fd; } 
        void closeServer();
        
};
#endif 