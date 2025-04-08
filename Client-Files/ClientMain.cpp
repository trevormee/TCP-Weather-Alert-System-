// ClientMain.cpp
#include "TcpClient.hpp"

#include <iostream>
#include <atomic>
#include <thread>

std::atomic<bool> running(true);
const int PORT = 60001;

void receiveLoop(TcpClient& client)
{
    while(running)
    {
        std::string server_response = client.receiveData();
        if(!server_response.empty())
        {
            std::cout << "\nServer: " << server_response << std::endl;
        }
        if(server_response.find("Server is shutting down. Disconnecting") != std::string::npos)
        {
            running = false;
            break;
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
        std::cout << "localhost resolved to 127.0.0.1\n";
    }

    TcpClient client(hostname, PORT);

    if (!client.connectToServer()) {
        std::cerr << "Failed to connect to server.\n";
        return 1;
    }
    
    std::thread receiver(receiveLoop, std::ref(client));

    while (running) {
        std::string message;
        std::getline(std::cin, message);
        
        if (message == "exit" || message == "0") {
            client.sendData("exit");
            running = false;
            break;
        }

        client.sendData(message.c_str());
    }

    receiver.join();
    
    client.closeConnection();

    return 0;
}