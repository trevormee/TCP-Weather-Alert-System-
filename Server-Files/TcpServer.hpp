/***************************************************************
  Student Name: Trevor Mee
  File Name: TcpServer.hpp
  Project 3 

  @brief Contains the function declarations for TcpServer class
         of the weather alert system. This class defines the 
         various functions related to the server side of the 
         client/server tcp paradigm including starting, running,
         and closing the server, along with handling client
         interactions.
***************************************************************/

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
#include <csignal>
#include <signal.h>

class TcpServer
{
    private:
        int server_fd;
        int port;
        const int NUM_CONCURRENT_CONNECTIONS = 10;

    public:
        // function declarations
        TcpServer(int port);
        ~TcpServer();
        bool startServer();
        void runServer();
        static void* handleClient(void* arg);
        int getServerFd() const { return server_fd; } 
        void closeServer();        
};
#endif 