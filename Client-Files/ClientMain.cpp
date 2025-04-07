
#include "TcpClient.hpp"

#include <iostream>


int main()
{

    std::string hostname;
    std::cout << "Enter Hostname: ";
    std::cin >> hostname; 
    std::cout << "\n" << std::endl;

    if(hostname == "localhost")
    {
        hostname = "127.0.0.1";
    }

    TcpClient client(hostname, 8080);
    //TcpClient client(hostname.c_str(), 8080); // Connect to the server on port 8080

    if (!client.connectToServer()) {
        std::cerr << "Failed to connect to server.\n";
        return 1;
    }

    std::string message;
    while(true) {
        std::cout << "You: ";
        std::cin.ignore();
        std::getline(std::cin, message);

        if(message == "exit"){
            break;
        }

        client.sendData(message.c_str());
        std::string response = client.receiveData();
        std::cout << "Server: " << response << "\n";
    }
    // const char* message = "Hello from client!";
    // client.sendData(message);

    // std::string response = client.receiveData();
    // std::cout << "Received from server: " << response << std::endl;

    client.closeConnection();
    return 0;
}