// Authentication.hpp

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

        bool isUserRegistered(const std::string& username, std::string& password);
        bool registerUser(const std::string& username, const std::string& password);
        void handleLoginRegister(int client_fd, std::map<std::string, User*>& onlineUsers, User*& currUser);
};
#endif