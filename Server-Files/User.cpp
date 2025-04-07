// User.cpp

#include "User.hpp"

User::User(const std::string& username, int socketNo) : username(username), socketNo(socketNo) {}


std::string User::getUsername() const
{
    return username;
}


int User::getSocketNo() const
{
    return socketNo;
}


std::vector<std::string> User::getMessages() const
{
    return messages;
}


std::set<std::string> User::getLocations() const
{
    return locations;
}


void User::addLocation(const std::string& location)
{
    locations.insert(location);
}


void User::removeLocation(const std::string& location) 
{
    locations.erase(location);
}


bool User::isSubscribedTo(const std::string& location) const
{
    return locations.find(location) != locations.end();
}


void User::addMessage(const std::string& message)
{
    if(messages.size() == NUM_CONCURRENT_CONNECTIONS)
    {
        messages.erase(messages.begin());
    }

    messages.push_back(message);
}