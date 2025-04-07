// TcpServer.cpp
#include "TcpServer.hpp"

TcpServer::TcpServer(int port) : port(port), server_fd(-1)
{
    startServer();
}


TcpServer::~TcpServer()
{
    closeServer();
}


bool TcpServer::startServer()
{
    // create the server socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("ERROR: Failed to create server socket. Exiting...");
        return false;
    }


    // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = INADDR_ANY; // Bind to any address
    server_address.sin_port = htons(port); 

    // bind the socket to the address and port
    if(bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) 
    {
        perror("Failed to bind the server socket. Exiting...");
        close(server_fd);
        return false;
    }

    std::cout << "bind done" << std::endl;

    // start listening for incoming connections
    if(listen(server_fd, NUM_CONCURRENT_CONNECTIONS) < 0) 
    {
        perror("Failed to listen on the server socket. Exiting...");
        close(server_fd);
        return false;
    }

    std::cout << "Waiting for incoming connections..." << std::endl;

    return true;
}


void TcpServer::runServer()
{
    if (server_fd < 0) 
    {
        std::cerr << "Error: server_fd" << std::endl;
        return;
    }

    while (true) 
    {
        // accept incoming connections
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_addr_len);

        if (client_fd < 0) 
        {
            perror("Failed to accept incoming connection");
            continue; 
        }

        std::cout << "Connection accepted" << std::endl;

        handleClient(client_fd);

        //close(client_fd); 
    }
    //close(client_fd); 

}

void TcpServer::handleClient(int client_fd)
{
    char buffer[1024] = {0};

    if(client_fd < 0)
    {
        perror("Error: Invalid client_fd");
    }

    // receive data from client
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if(bytes_read < 0)
    {
        perror("FAILED: Error reading from client socket");
        return; // Exit the function if read fails
    }

    std::cout << "Received data from client: " << std::string(buffer, bytes_read) << std::endl;

    std::string reply = "Hello from server!";
    send(client_fd, reply.c_str(), reply.size(), 0);

}


void TcpServer::closeServer()
{
    if (server_fd > 0) 
    {
        close(server_fd);
        server_fd = -1; 
    }
}