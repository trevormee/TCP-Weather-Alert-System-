/***************************************************************
  Student Name: Trevor Mee
  File Name: User.cpp
  Project 3 

  @brief Contains the function definitions for the User class
         of the weather alert system. This class defines the 
         properties and behaviors of a connected user, including
         their username, subscribed locations, socket number,
         and receieved messages
***************************************************************/

#include "User.hpp"

/*
    @brief Parameterized constructor to init a User object
    @param username: the user's username
    @param socketNo: the socket number assoc with the client
*/
User::User(const std::string& username, int socketNo) : username(username), socketNo(socketNo) {}


/*
    @brief retrieves the user's username
    @return the username
*/
std::string User::getUsername() const
{
    return username;
}

/*
    @brief retrieves the socket # associated with the user
    @return the socket number
*/
int User::getSocketNo() const
{
    return socketNo;
}


/*
    @brief retrieves the list user's messages
    @return a vector of messages
*/
std::vector<std::string> User::getMessages() const
{
    return messages;
}


/*
    @brief retreives the user's subscribed locations
    @return a set of locations
*/
std::set<std::string> User::getLocations() const
{
    return locations;
}


/*
    @brief adds a location to the list of subscribed locations
           for the user
    @param location: the name of the location to add 
    @return N/A
*/
void User::addLocation(const std::string& location)
{
    locations.insert(location);
}


/*
    @brief removes a location from the list of subcribed 
           locations for a user
    @param location: the name of the location to remove
    @return N/A

*/
void User::removeLocation(const std::string& location) 
{
    locations.erase(location);
}


/*
    @brief checks if a user is subscribed to a given location
    @param location: the name of the location to check for
    @return true if subscribed, false if not
*/
bool User::isSubscribedTo(const std::string& location) const
{
    return locations.find(location) != locations.end();
}


/*
    @brief adds a message to the user's message list
    @param message: the message to be added
    @return N/A
*/
void User::addMessage(const std::string& message)
{
    // remove oldest message if we reach max msgs (10)
    if(messages.size() == MAX_MESSAGES)
    {
        messages.erase(messages.begin());
    }

    messages.push_back(message);
}