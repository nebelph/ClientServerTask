#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // для sockaddr_in, inet_pton
#include <unistd.h> // для close, sleep
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <mutex>

class ServerConnection {
public:
	ServerConnection();
	~ServerConnection();
	
	void connectToServer();
	void sendData(const std::string&);
	bool isConnected();
private:
	std::string ip_;
	int port_;
	
	int clientSocket_;
	struct sockaddr_in serverAddr_;
	socklen_t serverAddrLen_;
	
	std::mutex connMutex_;
	
	void createSocket();
	void disconnect();
};
