
#include "TcpClient.hpp"

#include <iostream>
#include <atomic>
#include <thread>

std::atomic<bool> running(true);

void receiveLoop(TcpClient& client)
{
    while(running)
    {
        std::string server_response = client.receiveData();
        if(!server_response.empty())
        {
            std::cout << "\nServer: " << server_response << std::endl;
        }
        if(server_response == "exit")
        {
            running = false;
        }
    }
}

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

    // std::string message;
    // while(true) {
    //     std::cout << "You: ";
    //     std::cin.ignore();
    //     std::getline(std::cin, message);

    //     if(message == "exit"){
    //         break;
    //     }

    //     client.sendData(message.c_str());
    //     std::string response = client.receiveData();
    //     std::cout << "Server: " << response << "\n";
    // }
    
    std::thread receiver(receiveLoop, std::ref(client));

    while (running) {
        std::string message;
        std::cout << "> ";
        std::getline(std::cin, message);
        client.sendData(message.c_str());

        if (message == "exit") {
            running = false;
            break;
        }
    }

    receiver.join();

    client.closeConnection();
    return 0;
}