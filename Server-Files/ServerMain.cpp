/***************************************************************
  Student Name: Trevor Mee
  File Name: ServerMain.cpp
  Project 3 

  @brief Contains the main function for starting and running 
         the tcp server
***************************************************************/

#include "TcpServer.hpp"
#include <iostream>

const int PORT = 60001;

int main() 
{

    TcpServer server(PORT); 
    server.runServer();
    return 0;
}