// ServerMain.cpp
#include "TcpServer.hpp"
#include <iostream>
const int PORT = 60001;

int main() 
{

    TcpServer server(PORT); 
    server.runServer();
    return 0;
}