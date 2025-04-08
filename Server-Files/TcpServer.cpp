/***************************************************************
  Student Name: Trevor Mee
  File Name: TcpServer.cpp
  Project 3 

  @brief Contains the function definitions for TcpServer class
         of the weather alert system. This class defines the 
         various functions related to the server side of the 
         client/server tcp paradigm including starting, running,
         and closing the server, along with handling client
         interactions.
***************************************************************/


#include "TcpServer.hpp"

// struct to hold a client's properties (only the file descriptor for now)
struct ClientHandlerArgs{
    int client_fd;
};

std::map<std::string, User*> onlineUsers;
std::map<std::string, std::set<std::string>> locationSubscribers;
std::set<int> clientSockets;
TcpServer* instance = nullptr;

/*
    @brief handles graceful shutdown of the server
    @param signal: signal received
    @return N/A
*/
void signalHandler(int signal)
{
    std::cout << "\nReceived signal: " << signal << ", shutting server down" << std::endl;
    if(instance) {
        instance->closeServer();
    }
    exit(signal);
}

/*
    @brief Parameterized constructor to init the TcpServer object,
           sets up signal handling, and start the server
    @param port: the port # to bind the server to
*/
TcpServer::TcpServer(int port) : port(port), server_fd(-1)
{
    signal(SIGINT, signalHandler);
    instance = this;
    startServer();
}

/*
    @brief Destructor for the TcpServer object to shut down
           the server
*/
TcpServer::~TcpServer()
{
    closeServer();
}

/*
    @brief Init. the tcp server socket, bind it to a port,
           and begin listening for connections
    @return true if server successfully starts, false if not
*/
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
    server_address.sin_addr.s_addr = INADDR_ANY; 
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

/*
    @brief Main server loop that accepts and handles concurrent clients
           using threading
    @return N/A
*/
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

        // add the client to the clientSockets set
        clientSockets.insert(client_fd);
        std::cout << "Connection accepted" << std::endl;

        ClientHandlerArgs* args = new ClientHandlerArgs{client_fd};

        pthread_t tid;

        // create a new thread for each client connection
        if(pthread_create(&tid, nullptr, TcpServer::handleClient, args) != 0)
        {
            perror("Error: Failed to create thread for client");
            close(client_fd);
            delete args;
        }
        else {
            pthread_detach(tid);
        }
    }

}


/*
    @brief handles client interactions 
    @param arg: pointer to ClientHandlerArgs containing client socket info
    @return nullptr
*/
void* TcpServer::handleClient(void* arg)
{
    ClientHandlerArgs* args = static_cast<ClientHandlerArgs*>(arg);
    int client_fd = args->client_fd;
    delete args;
    Authentication auth;
    User* currUser = nullptr;

    // login or register a user
    auth.handleLoginRegister(client_fd, onlineUsers, currUser);

    char buffer[1024] = {0};

    while(true)
    {
        // prompt the user with a list of weather alert system options
        std::string user_options = "\n1 Subscribe to a location\n"
                                    "2 Unsubscribe from a location\n"
                                    "5 See all the locations you are subsribed to\n"
                                    "8 Change password\n"
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

        // add location subscription route
        if(client_data == "1")
        {
            // prompt the user for the name of the location they want to subscribe to
            std::string input_location_prompt = "Insert the location you want to subscribe to: ";
            send(client_fd, input_location_prompt.c_str(), input_location_prompt.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string location(buffer);
            location.erase(std::remove(location.begin(), location.end(), '\n'), location.end());

            // add the location to the list of user's location subscriptions
            currUser->addLocation(location);
            locationSubscribers[location].insert(currUser->getUsername());
            
            std::string successfull_location_add = "Successfully subscribed. Select an option: ";
            send(client_fd, successfull_location_add.c_str(), successfull_location_add.size(), 0);
        }
        // remove location subscription route
        else if(client_data == "2")
        {
            // prompt the user for the name of the location they want to unsubscribe from
            std::string unsubcribe_prompt = "Enter the location you want to unsubscribe from: ";
            send(client_fd, unsubcribe_prompt.c_str(), unsubcribe_prompt.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string location(buffer);
            location.erase(std::remove(location.begin(), location.end(), '\n'), location.end());

            // remove the location from a user's list of subscriptions
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
        // output location subscriptions route
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
        // change password route
        else if(client_data == "8"){
            // prompt the user for their current password
            std::string currPwPrompt = "Enter your current password: ";
            send(client_fd, currPwPrompt.c_str(), currPwPrompt.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string currPw(buffer);
            currPw.erase(std::remove(currPw.begin(), currPw.end(), '\n'), currPw.end());
            
            std::string storedPw;
            
            // check if the input password matches the user's current password
            if(auth.isUserRegistered(currUser->getUsername(), storedPw) && storedPw == currPw)
            {
                // prompt user for their new password
                std::string changePasswordPrompt = "Enter your new password: ";
                send(client_fd, changePasswordPrompt.c_str(), changePasswordPrompt.size(), 0);
                memset(buffer, 0, sizeof(buffer));
                read(client_fd, buffer, sizeof(buffer));
                std::string newPw(buffer);
                newPw.erase(std::remove(newPw.begin(), newPw.end(), '\n'), newPw.end());

                // update a user's password
                if(auth.updatePassword(currUser->getUsername(), newPw))
                {
                    std::string successfulPwChange = "Password successfully changed.\n";
                    send(client_fd, successfulPwChange.c_str(), successfulPwChange.size(), 0);
                }
                else {
                    std::string currPwNotEqualPrompt = "Current password is incorred.\n";
                    send(client_fd, currPwNotEqualPrompt.c_str(), currPwNotEqualPrompt.size(), 0);
                }
            }
        }
        // terminate route
        else if(client_data == "0" || client_data == "exit")
        {
            std::cout << "Client " << currUser->getUsername() << " has disconnected.\n";
            break;
        }
        // incorrect option route
        else {
            std::string invalid_choice = "Invalid choice. Try again\n";
            send(client_fd, invalid_choice.c_str(), invalid_choice.size(), 0);
        }
    }

    // clean up
    if(currUser){
        onlineUsers.erase(currUser->getUsername());
        delete currUser;
    }

    clientSockets.erase(client_fd);
    close(client_fd);
    pthread_exit(nullptr);
    return nullptr;
}

/*
    @brief Closes the Tcp server and notifies clients
    @return N/A
*/
void TcpServer::closeServer()
{
    std::cout << "Closing server...\n";

    // notify clients
    for(int x : clientSockets)
    {
        std::string serverShutdownMsg = "Server is shutting down. Disconnecting...\n";
        send(x, serverShutdownMsg.c_str(), serverShutdownMsg.size(), 0);
        close(x);
    }

    clientSockets.clear();

    // close the server
    if (server_fd > 0) 
    {
        close(server_fd);
        server_fd = -1; 
    }
}