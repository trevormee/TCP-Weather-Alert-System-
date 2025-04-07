// User.hpp

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <set>

class User
{
    private:
        std::string username;
        std::string password;
        std::set<std::string> locations;
        int socketNo;
        std::vector<std::string> messages;
        

    public:
        const int NUM_CONCURRENT_CONNECTIONS = 10;

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