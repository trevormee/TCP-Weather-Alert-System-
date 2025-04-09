## Overview
This project is a multi-threaded TCP Weather Alert System implemented in C++. It consists of a server that handles multiple clients concurrently and a client application that allows uers to interact with the system through a terminal-based interface.

## Compilation & Usage
1) Open two(or more) terminals.
2) In each terminal, navigate into the root directory of this project.
3) Type ``make`` into one terminal. This will generate two executables: ``client`` and ``server``
4) In one terminal, type ``./server``. This will start the TCP server.
5) In the other terminal(s), type ``./client``. This will prompt the user to connect with the server.
6) Once running, follow the on-screen prompts to interact with the TCP server as a client!

## Current Functionality
The user currently has the following options for the weather alert system...
- **Initial Connection**
    - The user will be prompted to enter the hostname/IP address of the server to connect to. For simplicity you can just type ``localhost`` and it will connect you to the IP address at 127.0.0.1.
- **Pre-login**
  - `1` Login
    - Prompts the user to enter their username
    - Prompts the user to enter ther password  
    - **Note:** I currently have two registered users already stored in ``users.txt``. Feel free to use these users when testing my project!
        - User1 Credentials
            - Username: trevor
            - Password: Password1
        - User2 Credentials
            - Username: newUser
            - Password: newUserPw
  - `2` Register  
    - Prompts the user to create their username
    - Prompts the user to create their password
  -  `exit`
     - Terminates the client's connection with the server

- **Post-login**
  - `1` Subscribe to a location  
    - Prompts the user to enter the name of the location they want to subscribe to
  - `2` Unsubscribe from a location  
    - Prompts the user to enter the name of the location they want to unsubcribe from
  - `5` See all the locations you are subscribed to 
    - Displays the list of location subscriptions of a user
  - `8` Change password  
    - Prompts the user to enter their current password
    - Prompts the user to enter their new password
  - `0` Quit 
    - Terminates the client's connection with the server


## Demos
I have provided two demo videos located in the 'demos' directory of this project.
- [Current-Functionality.mp4](demos/Current-Functionality.mp4): Is a screen recording of one client interacting with one server in seperate terminals along with 'users.txt' displayed on screen. This video demos the functionalities of logging in, subscribing and unsubscribing from locations, viewing all location subscriptions for a user, changing a user's password, and terminating the session altogether.
- [Concurrent-Clients-Register.mp4](demos/Concurrent-Clients-Register.mp4): Is a screen recording of two concurrent clients interacting with the server. You will see one client terminal log in in a similar manner as Current-Functionality.mp4. In the other client terminal, I demonstate the registration functionality of the project as well.