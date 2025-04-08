/***************************************************************
  Student Name: Trevor Mee
  File Name: Authentication.hpp
  Project 3 

  @brief Contains the function declarations for the Authentication
         class of the weather alert system. This class defines 
         the various functions related to handling user login, 
         registration, and checking if a user is registered.
***************************************************************/

#ifndef AUTHENTICATION_HPP
#define AUTHENTICATION_HPP

#include "User.hpp"

#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class Authentication
{
    private:

    public:
        const std::string USERS_TXT = "users.txt";

        // function declarations
        bool isUserRegistered(const std::string& username, std::string& password);
        bool registerUser(const std::string& username, const std::string& password);
        bool updatePassword(const std::string& username, const std::string& newPassword);
        void handleLoginRegister(int client_fd, std::map<std::string, User*>& onlineUsers, User*& currUser);
};
#endif