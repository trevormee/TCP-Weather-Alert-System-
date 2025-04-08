/***************************************************************
  Student Name: Trevor Mee
  File Name: Authentication.cpp
  Project 3 

  @brief Contains the function definitions for the Authentication
         class of the weather alert system. This class defines 
         the various functions related to handling user login, 
         registration, and checking if a user is registered.
***************************************************************/
#include "Authentication.hpp"

/*
    @brief checks if a user is registered in users.txt
    @param username: the user's given username to check
    @param password: the uesr's given password to check
    @return true if a user is found, false if not found
*/
bool Authentication::isUserRegistered(const std::string& username, std::string& password)
{
    // open users.txt to read from
    std::ifstream fileRead(USERS_TXT);
    std::string line;

    if(!fileRead.is_open())
    {
        std::cerr << "Error: Could not open users.txt\n";
        return false;
    }

    // search for the user
    while(std::getline(fileRead, line))
    {
        std::istringstream iss(line);
        std::string uname, pw;
        if(std::getline(iss, uname, ',') && std::getline(iss, pw)){
            
            uname.erase(std::remove(uname.begin(), uname.end(), '\n'), uname.end());
            pw.erase(std::remove(pw.begin(), pw.end(), '\n'), pw.end());
            if(uname == username){
                password = pw;
                return true;
            }
        }
    }

    return false;
}


/*
    @brief registers a user into users.txt
    @param username: the user's given username to register with
    @param password: the user's given password to register with
    @return true if a user is successfully registered, false if not
*/
bool Authentication::registerUser(const std::string& username, const std::string& password)
{
    std::string pw;
    if(isUserRegistered(username, pw))
    {
        return false;       // user already registered
    }
    
    // open users.txt to append to
    std::ofstream fileWrite(USERS_TXT, std::ios::app);

    if(!fileWrite.is_open())
    {
        std::cerr << "error: could not write to users.txt\n";
        return false;
    }
    
    // write the user's credentials to users.txt
    fileWrite << username << "," << password << "\n";

    fileWrite.close();

    return true;
}


/*
    @brief updates an existing user's password
    @param username: the user's username
    @param newPassword: the requested new password for the user
    @return true if the user's password is updated successfully,
            false if not
*/
bool Authentication::updatePassword(const std::string& username, const std::string& newPassword)
{
    // open users.txt to read from
    std::ifstream fileRead(USERS_TXT);

    if(!fileRead.is_open()){
        return false;
    }

    std::string line;
    std::string updateCredentials;
    bool updatedYet = false;

    // search for the user
    while(std::getline(fileRead, line))
    {
        std::istringstream iss(line);
        std::string uname, pw;
        if(std::getline(iss, uname, ',') && std::getline(iss, pw)){

            if(uname == username){
                updateCredentials += uname + "," + newPassword + "\n";
                updatedYet = true;  
            } else {
                updateCredentials += line + "\n";
            }
        }
    }

    fileRead.close();

    if(!updatedYet){
        return false;
    }

    // open users.txt to write to
    std::ofstream fileWrite(USERS_TXT);

    if(!fileWrite.is_open())
    {
        std::cerr << "Could not open users.txt" << std::endl;
        return false;
    }

    // update the user's credentials in users.txt
    fileWrite << updateCredentials;

    fileWrite.close();
    return true;
}

/*
    @brief handles the various functions associated with user authentication
    @param onlineUsers: a list of all users(clients) currently on the server
    @param currUser: the current user 
    @return N/A
*/
void Authentication::handleLoginRegister(int client_fd, std::map<std::string, User*>& onlineUsers, User*& currUser)
{
    char buffer[1024] = {0};

    while(true)
    {
        // prompt the user with a list of options
        std::string user_options = "Welcome\nPress 1 to login\nPress 2 to register\nType 'exit' to quit\n";
        send(client_fd, user_options.c_str(), user_options.size(), 0);
        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, sizeof(buffer));
        std::string choice(buffer);
        choice.erase(std::remove(choice.begin(), choice.end(), '\n'), choice.end());

        // login route
        if(choice == "1")   
        {
            // prompt user for their username
            std::string enter_username = "Enter username: ";
            send(client_fd, enter_username.c_str(), enter_username.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string username(buffer);
            username.erase(std::remove(username.begin(), username.end(), '\n'), username.end());
            
            // prompt user for their password
            std::string enter_password = "Enter password: ";
            send(client_fd, enter_password.c_str(), enter_password.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string password(buffer);
            password.erase(std::remove(password.begin(), password.end(), '\n'), password.end());

            std::string users_txt_password;
            // login user
            if(isUserRegistered(username, users_txt_password) && users_txt_password == password)
            {
                currUser = new User(username, client_fd);
                onlineUsers[username] = currUser;
                std::string successfull_login = "Successfully Logged In";
                send(client_fd, successfull_login.c_str(), successfull_login.size(), 0);
                break;
            }
            else {
                std::string failed_login = "Failed Logging In. Try again\n";
                send(client_fd, failed_login.c_str(), failed_login.size(), 0);
            }
            
        }
        // register a user
        else if(choice == "2")  
        {
            // prompt user to enter a username
            std::string enter_username = "Enter username: ";
            send(client_fd, enter_username.c_str(), enter_username.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string username(buffer);
            username.erase(std::remove(username.begin(), username.end(), '\n'), username.end());
            
            // prompt user to enter a password
            std::string enter_password = "Enter password: ";
            send(client_fd, enter_password.c_str(), enter_password.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string password(buffer);
            password.erase(std::remove(password.begin(), password.end(), '\n'), password.end());

            // register the user
            if(registerUser(username, password))
            {
                currUser = new User(username, client_fd);
                onlineUsers[username] = currUser;
                std::string successfull_register = "Successfully Registed\n";
                send(client_fd, successfull_register.c_str(), successfull_register.size(), 0);
            }
            else {
                std::string fail = "Username already taken. Try again.\n";
                send(client_fd, fail.c_str(), fail.size(), 0);
            }
        } 
        // terminate user
        else if(choice == "exit")
        {
            std::cout << "Client disconnected\n";
            close(client_fd);
        }
        // reprompt the user to pick a different choice
        else {
            std::string invalid_choice = "Invalid choice. Try again\n";
            send(client_fd, invalid_choice.c_str(), invalid_choice.size(), 0);
        }
    }
}