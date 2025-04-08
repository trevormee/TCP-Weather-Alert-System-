/***************************************************************
  Student Name: Trevor Mee
  File Name: User.hpp
  Project 3 

  @brief Contains the function declarations for the User class
         of the weather alert system. This class defines the 
         properties and behaviors of a connected user, including
         their username, subscribed locations, socket number,
         and receieved messages
***************************************************************/

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <set>

class User
{
    private:
        // private instance variables
        std::string username;
        std::string password;
        std::set<std::string> locations;
        int socketNo;
        std::vector<std::string> messages;
        

    public:
        const int MAX_MESSAGES = 10;

        // function declarations
        User(const std::string& username, int socketNo);
        std::string getUsername() const;
        int getSocketNo() const;
        std::vector<std::string> getMessages() const;
        std::set<std::string> getLocations() const;
        void addLocation(const std::string& location);
        void removeLocation(const std::string& location);
        bool isSubscribedTo(const std::string& location) const;
        void addMessage(const std::string& message);
};
#endif