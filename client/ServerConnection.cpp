#include "ServerConnection.h"

ServerConnection::ServerConnection() : clientSocket_(-1) {
	std::ifstream file("../../serveraddr.json");
	nlohmann::json addr;
	file >> addr;
	file.close();
	ip_ = addr["ip"];
	port_ = addr["port"];

	serverAddr_.sin_family = AF_INET; // всегда равно AF_INET
	serverAddr_.sin_port = htons(port_); // номер порта который намерен занять процесс
	inet_pton(AF_INET, ip_.c_str(), &serverAddr_.sin_addr); // привязывает IP к сокету
	serverAddrLen_ = sizeof(serverAddr_);
}

ServerConnection::~ServerConnection() {
	disconnect();
}

void ServerConnection::createSocket() {
	clientSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket_ == -1)
		throw std::runtime_error("Ошибка при создании сокета.");
}

void ServerConnection::connectToServer() {
	if (clientSocket_ != -1)
		if (close(clientSocket_) == -1) std::cout << "Ошибка при закрытии сокета.\n";
	
	createSocket();
	
	std::cout << "Попытка подключения к серверу...\n";
	
	if (connect(clientSocket_, (struct sockaddr*)&serverAddr_, serverAddrLen_) == -1)
		std::cout << "Ошибка при установлении связи с сервером.\n";
	else
		std::cout << "Связь с сервером установлена.\n";
}

void ServerConnection::sendData(const std::string& data) {
	std::lock_guard<std::mutex> lock(connMutex_);
	
	if (!isConnected()) {
		std::cout << "Нет соединения с сервером.\n";
		return;
	}
	
	if (send(clientSocket_, data.c_str(), data.size(), 0) == -1)
		std::cout << "Ошибка при отправке данных.\n";
	else
		std::cout << "Данные отправлены.\n";
}

bool ServerConnection::isConnected() {
	if (getpeername(clientSocket_, (struct sockaddr*)&serverAddr_, &serverAddrLen_) == -1)
		return false;
	return true;
}

void ServerConnection::disconnect() {
	if (shutdown(clientSocket_, SHUT_RDWR) == -1)
		std::cout << "Ошибка при закрытии соединения.\n";
	if (close(clientSocket_) == -1)
		std::cout << "Ошибка при закрытии сокета.\n";
}
