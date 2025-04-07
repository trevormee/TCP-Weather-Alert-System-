// TcpServer.cpp
#include "TcpServer.hpp"

struct ClientHandlerArgs{
    int client_fd;
};

std::map<std::string, User*> onlineUsers;
std::map<std::string, std::set<std::string>> locationSubscribers;

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

        //handleClient(client_fd);
        ClientHandlerArgs* args = new ClientHandlerArgs{client_fd};
        pthread_t tid;

        if(pthread_create(&tid, nullptr, TcpServer::handleClient, args) != 0)
        {
            perror("ERROR: Failed to create thread for client");
            close(client_fd);
            delete args;
        }
        else {
            pthread_detach(tid);
        }

        //close(client_fd); 
    }
    //close(client_fd); 

}

void* TcpServer::handleClient(void* arg)
{
    ClientHandlerArgs* args = static_cast<ClientHandlerArgs*>(arg);
    int client_fd = args->client_fd;
    delete args;
    Authentication auth;
    User* currUser = nullptr;
    auth.handleLoginRegister(client_fd, onlineUsers, currUser);

    char buffer[1024] = {0};

    while(true)
    {
        std::string user_options = "\n1 Subscribe to a location\n"
                                    "2 Unsubscribe from a location\n"
                                    "5 See all the locations you are subsribed to\n"
                                    "0 Quit\n";
        send(client_fd, user_options.c_str(), user_options.size(), 0);
        memset(buffer, 0, sizeof(buffer));

        // receive data from client
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

        if(bytes_read <= 0)
        {
            perror("FAILED: Error reading from client socket");
            break; 
        }

        std::string client_data(buffer, bytes_read);
        client_data.erase(std::remove(client_data.begin(), client_data.end(), '\n'), client_data.end());
        std::cout << "Received from " << currUser->getUsername() << ": " << client_data << std::endl;

        if(client_data == "0")
        {
            std::string goodbye = "Goodbye!\n";
            send(client_fd, goodbye.c_str(), goodbye.size(), 0);
            break;
        }
        else if(client_data == "1")
        {
            std::string input_location_prompt = "Insert the location you want to subscribe to: ";
            send(client_fd, input_location_prompt.c_str(), input_location_prompt.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string location(buffer);
            location.erase(std::remove(location.begin(), location.end(), '\n'), location.end());

            currUser->addLocation(location);
            locationSubscribers[location].insert(currUser->getUsername());
            
            std::string successfull_location_add = "Successfully subscribed. Select an option: ";
            send(client_fd, successfull_location_add.c_str(), successfull_location_add.size(), 0);
        }
        else if(client_data == "2")
        {
            std::string unsubcribe_prompt = "Enter the location you want to unsubscribe from: ";
            send(client_fd, unsubcribe_prompt.c_str(), unsubcribe_prompt.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string location(buffer);
            location.erase(std::remove(location.begin(), location.end(), '\n'), location.end());

            if(currUser->isSubscribedTo(location)){
                currUser->removeLocation(location);
                locationSubscribers[location].erase(currUser->getUsername());
                std::string unsubcribed_msg = "Unsubcribed from " + location + "\n";
                send(client_fd, unsubcribed_msg.c_str(), unsubcribed_msg.size(), 0);
            }else {
                std::string user_not_subscribed_msg = "You are not subscribed to that location.\n";
                send(client_fd, user_not_subscribed_msg.c_str(), user_not_subscribed_msg.size(), 0);
            }

        }
        else if(client_data == "5")
        {
            auto locationSubs = currUser->getLocations();
            if(locationSubs.empty())
            {
                std::string no_location = "No location subscriptions yet";
                send(client_fd, no_location.c_str(), no_location.size(), 0);
            }else {     // user is subscribed to at least 1 location
                std::string user_location_subs = "You are subscribed to:\n";
                int i = 0;
                for(const auto& x : locationSubs)
                {
                    user_location_subs += std::to_string(i++) + " " + x + "\n";
                }
                send(client_fd, user_location_subs.c_str(), user_location_subs.size(), 0);
            }
        }
        else {
            std::string invalid_choice = "Invalid choice. Try again\n";
            send(client_fd, invalid_choice.c_str(), invalid_choice.size(), 0);
        }

        // std::string server_echo = "Server receievd: " + client_data + "\n";
        // send(client_fd, server_echo.c_str(), server_echo.size(), 0);

        // std::string server_reply;
        // std::getline(std::cin, server_reply);

        // send(client_fd, server_reply.c_str(), server_reply.size(), 0);
        // if(server_reply == "exit")
        // {
        //     std::cout << "server ended chat. Disconnecting..." << std::endl;
        //     break;
        // }

        // memset(buffer, 0, sizeof(buffer));
    }

    if(currUser){
        onlineUsers.erase(currUser->getUsername());
    }

    
    close(client_fd);
    pthread_exit(nullptr);
    return nullptr;
}


void TcpServer::closeServer()
{
    if (server_fd > 0) 
    {
        close(server_fd);
        server_fd = -1; 
    }
}