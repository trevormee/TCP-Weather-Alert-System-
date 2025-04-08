# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g -pthread

# Directories
CLIENT_DIR = Client-Files
SERVER_DIR = Server-Files

# Client files
CLIENT_SRCS = $(CLIENT_DIR)/TcpClient.cpp $(CLIENT_DIR)/ClientMain.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
CLIENT_TARGET = client

# Server files
SERVER_SRCS = $(SERVER_DIR)/TcpServer.cpp \
			  $(SERVER_DIR)/ServerMain.cpp \
			  $(SERVER_DIR)/Authentication.cpp \
			  $(SERVER_DIR)/User.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
SERVER_TARGET = server

# Default target
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# Build client executable
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build server executable
$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS) $(CLIENT_TARGET) $(SERVER_TARGET)

.PHONY: all clean
