#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // для sockaddr_in
#include <unistd.h> // для close, sleep
#include <fstream>
#include <nlohmann/json.hpp>
#include <fcntl.h> // для O_NONBLOCK

class Server {
public:
	Server();
	~Server();
	
	void start();
private:
	int port_;

	int serverSocket_;
	int clientSocket_;
	struct sockaddr_in serverAddr_;
	socklen_t serverAddrLen_;
	struct sockaddr_in clientAddr_;
	socklen_t clientAddrLen_;
	
	void createSocket();
	void bindSocket();
	void listenForClients();
	void acceptClient();
	void handleData(char*);
	void receiveData();
	void stop();
};
