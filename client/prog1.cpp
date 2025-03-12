#include "BufferManager.h"
#include "ServerConnection.h"

#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>

void makeConnection(ServerConnection& serverConnection) {
	while (true) {
		if (!serverConnection.isConnected())
			serverConnection.connectToServer();
		sleep(1);
	}
}

void inputData(BufferManager& bufferManager) {
	while (true) {
		std::string input;
		std::getline(std::cin, input);
		
		if (input.size() > 64 || input.empty()) {
			std::cout << "Неправильно введённые данные.\n";
			continue;
		}
		
		bool valid = true;
		for (const auto& ch : input) {
			if (!(ch >= 65 && ch <= 90 || ch >= 97 && ch <= 122)) {
				std::cout << "Неправильно введённые данные.\n";
				valid = false;
				break;
			}
		}
		if (!valid)
			continue;
		
		bufferManager.setString(input);
	}
}

void processData(BufferManager& bufferManager, ServerConnection& serverConnection) {
	while (true) {
		bufferManager.waitForData();
		
		std::map<char, unsigned> charCount = bufferManager.getProcessedData();	
		for (const auto& i : charCount) {
			std::cout << "'" << i.first << "': " << i.second << std::endl;
		}
		bufferManager.clearBuffer();
	
		nlohmann::json jsonCharCount = charCount;
		serverConnection.sendData(jsonCharCount.dump());
	}
}

int main(){
	BufferManager bufferManager;
	ServerConnection serverConnection;
	
	std::thread t1(makeConnection, std::ref(serverConnection));
	std::thread t2(inputData, std::ref(bufferManager));
	std::thread t3(processData, std::ref(bufferManager), std::ref(serverConnection));
	
	t1.join();
	t2.join();
	t3.join();
	
	return 0;
}
