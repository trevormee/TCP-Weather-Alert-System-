
#include "TcpServer.hpp"
// #include "TcpServer.cpp"
#include <iostream>

int main() 
{
    TcpServer server(8080); // Start the server on port 8080
    server.runServer();
    return 0;
}