/***************************************************************
  Student Name: Trevor Mee
  File Name: TcpClient.cpp
  Project 3 

  @brief Contains the function definitions for TcpClient class
         of the weather alert system. This class defines the 
         various functions related to the client side of the 
         client/server tcp paradigm including connecting to 
         a server, reading from a server, and writing to a server
***************************************************************/

#include "TcpClient.hpp"

/*
    @brief Parameterized constructor for the TcpClient object
    @param ip: IP address of the server to connect to
    @param port: port # of the server to connect to
*/
TcpClient::TcpClient(const std::string& ip, int port) : server_ip(ip), server_port(port), client_fd(-1) {}


/*
    @brief Destructor for the TcpClient object to close a client
           connection
*/
TcpClient::~TcpClient()
{
    closeConnection();
}

/*
    @brief Establishes a connection to the server
    @return true if connection was successful, false if not
*/
bool TcpClient::connectToServer()
{
    // create the client socket
    if(client_fd = socket(AF_INET, SOCK_STREAM, 0); client_fd < 0) 
    {
        perror("Failed to create client socket. Exiting...");
        return false;
    }

    std::cout << "Client socket created successfully." << std::endl;

    // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(server_port); 

    if(inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) 
    {
        perror("ERROR: Invalid IP Address. Closing client and exiting...");
        close(client_fd);
        return false;
    }

    // connect to server
    if(connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) 
    {
        perror("FAILED: Error trying to connect to the server. Exiting...");
        close(client_fd); 
        client_fd = -1; 
        return false;
    }
    std::cout << "Client connected to server successfully." << std::endl;

    return true;
}

/*
    @brief Sends data to the connected server
    @param data: the data to be sent
    @return number of bytes successfully sent, 
            returns -1 if sending fails
*/
int TcpClient::sendData(const char* data)
{
    // ensure the client socket is connected before sending data
    if (client_fd <= 0) 
    {
        perror("Socket is not connected. Cannot send data.");
        return -1; 
    }

    // send the data to the server
    int bytes_sent = send(client_fd, data, strlen(data), 0);
    if (bytes_sent < 0)
    {
        perror("FAILED: Error could not send data to the server.");
        return -1; 
    }
    
    return bytes_sent; 
}

/*
    @brief Receives data from the server
    @return the data received from the server,
            returns empty string if not data receieved
*/
std::string TcpClient::receiveData()
{
    char buffer[1024] = {0};
    
    // ensure the client socket is connected before sending data
    if (client_fd <= 0) 
    {
        perror("Socket is not connected. Cannot receive data.");
        return ""; 
    }

    // receive data from server
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0); 

    if(bytes_received < 0)
    {
        perror("FAILED: Error receiving data from the server.");
        return "";
    }

    return std::string(buffer, bytes_received); 
}

/*
    @brief gets the client socket file descriptor
    @return the file descriptor of the client socket
*/
int TcpClient::getClientFd()
{
    return client_fd; 
}

/*
    @brief Closes the connection to the server
    @return N/A
*/
void TcpClient::closeConnection() 
{
    if (client_fd > 0)
    {
        close(client_fd);
        client_fd = -1; 
    }
}