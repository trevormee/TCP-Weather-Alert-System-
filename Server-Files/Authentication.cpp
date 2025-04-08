// Authentication.cpp

#include "Authentication.hpp"

bool Authentication::isUserRegistered(const std::string& username, std::string& password)
{
    std::ifstream fileRead(USERS_TXT);
    std::string line;

    if(!fileRead.is_open())
    {
        std::cerr << "Error: Could not open users.txt\n";
        return false;
    }

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

    //fileRead.close();
    return false;
}

bool Authentication::registerUser(const std::string& username, const std::string& password)
{
    std::string pw;
    if(isUserRegistered(username, pw))
    {
        return false;       // user already registered
    }
    
    std::ofstream fileWrite(USERS_TXT, std::ios::app);
    if(!fileWrite.is_open())
    {
        std::cerr << "error: could not write to users.txt\n";
        return false;
    }
    
    fileWrite << username << "," << password << "\n";

    fileWrite.close();
    return true;
}


bool Authentication::updatePassword(const std::string& username, const std::string& newPassword)
{
    std::ifstream fileRead(USERS_TXT);

    if(!fileRead.is_open()){
        return false;
    }

    std::string line;
    std::string updateCredentials;
    bool updatedYet = false;

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

    std::ofstream fileWrite(USERS_TXT);

    if(!fileWrite.is_open())
    {
        return false;
    }

    fileWrite << updateCredentials;
    fileWrite.close();
    return true;
}

void Authentication::handleLoginRegister(int client_fd, std::map<std::string, User*>& onlineUsers, User*& currUser)
{
    char buffer[1024] = {0};
    while(true)
    {
        std::string user_options = "Welcome\nPress 1 to login\nPress 2 to register\nType 'exit' to quit\n";
        send(client_fd, user_options.c_str(), user_options.size(), 0);
        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, sizeof(buffer));
        std::string choice(buffer);
        choice.erase(std::remove(choice.begin(), choice.end(), '\n'), choice.end());

        if(choice == "1")   // login route
        {
            std::string enter_username = "Enter username: ";
            send(client_fd, enter_username.c_str(), enter_username.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string username(buffer);
            username.erase(std::remove(username.begin(), username.end(), '\n'), username.end());

            std::string enter_password = "Enter password: ";
            send(client_fd, enter_password.c_str(), enter_password.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string password(buffer);
            password.erase(std::remove(password.begin(), password.end(), '\n'), password.end());

            std::string users_txt_password;
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
        else if(choice == "2")  // register route
        {
            std::string enter_username = "Enter username: ";
            send(client_fd, enter_username.c_str(), enter_username.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string username(buffer);
            username.erase(std::remove(username.begin(), username.end(), '\n'), username.end());

            std::string enter_password = "Enter password: ";
            send(client_fd, enter_password.c_str(), enter_password.size(), 0);
            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            std::string password(buffer);
            password.erase(std::remove(password.begin(), password.end(), '\n'), password.end());

            if(registerUser(username, password))
            {
                currUser = new User(username, client_fd);
                onlineUsers[username] = currUser;
                std::string successfull_register = "Successfully Registed\n";
                send(client_fd, successfull_register.c_str(), successfull_register.size(), 0);
                //break;
            }else {
                std::string fail = "Username already taken. Try again.\n";
                send(client_fd, fail.c_str(), fail.size(), 0);
            }
        } else if(choice == "exit")
        {
            std::cout << "Client disconnected\n";
            close(client_fd);
        }
        
        else {
            std::string invalid_choice = "Invalid choice. Try again\n";
            send(client_fd, invalid_choice.c_str(), invalid_choice.size(), 0);
        }
    }
}