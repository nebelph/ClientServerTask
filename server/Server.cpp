#include "Server.h"

Server::Server() : serverSocket_(-1), clientSocket_(-1) {
	std::ifstream file("../../serveraddr.json");
	nlohmann::json addr;
	file >> addr;
	file.close();
	port_ = addr["port"];
	
	createSocket();
	
	int opt = 1;
	setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
 
	
	serverAddr_.sin_family = AF_INET;  // всегда равно AF_INET
	serverAddr_.sin_port = htons(port_); // номер порта который намерен занять процесс
	serverAddr_.sin_addr.s_addr = INADDR_ANY; // привязывает все локальные интерфейсы к сокету
	serverAddrLen_ = sizeof(serverAddr_);
	
	clientAddrLen_ = sizeof(clientAddr_);
	
	bindSocket();
	listenForClients();
}

Server::~Server() {
	stop();
}

void Server::createSocket() {
	serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket_ == -1)
		throw std::runtime_error("Ошибка при создании сокета.");
}

void Server::bindSocket() {
	if (bind(serverSocket_, (struct sockaddr*)&serverAddr_, serverAddrLen_) == -1)
		throw std::runtime_error("Ошибка при привязке локального адреса сокету.");
	else
		std::cout << "Сокету успешно привязан локальный адрес.\n";
}

void Server::listenForClients() {
	if (listen(serverSocket_, 5) == -1)
		throw std::runtime_error("Ошибка при ожидании соединения.");
	else
		std::cout << "Сервер слушает соединения на порту " << port_ << ".\n";
}

void Server::acceptClient() {
	fcntl(serverSocket_, F_SETFL, O_NONBLOCK); // операции чтения и записи не блокируют выполнение программы, если данные в данный момент недоступны
	
	while (true) {
		clientSocket_ = accept(serverSocket_, (struct sockaddr*)&clientAddr_, &clientAddrLen_);
		if (clientSocket_ == -1) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) { // сокет маркирован как неблокирующий, но нет ни одного соединения, которое можно было бы принять
				usleep(100000);
				continue;
			}
			throw std::runtime_error("Ошибка принятия соединения на сокете.");
		}
		std::cout << "Клиент успешно соединён.\n";
		
		receiveData();
	}
}

void Server::handleData(char* buf) {
	nlohmann::json j = nlohmann::json::parse(buf);
	std::map data = j.get<std::map<char, unsigned>>();
	std::string newstr = "";
	std::vector<std::pair<char, unsigned>> v(data.begin(), data.end());
	std::sort(v.begin(), v.end(), [](const auto& left, const auto& right) {
		return left.second < right.second;
	});
	for (const auto& [first, second] : v) {
		for (unsigned i = 0; i < second; i++) {
			newstr += first;
		}
	}
    std::cout << "Получено: " << newstr << std::endl;
}

void Server::receiveData() {
	char buffer[1024];
	while (true) {
		int bytesReceived = recv(clientSocket_, buffer, sizeof(buffer) - 1, 0);
		if (bytesReceived <= 0) {
			std::cout << "Клиент отключился.\n";
			break;
		}
		buffer[bytesReceived] = '\0';
		
		handleData(buffer);
	}
	
	if (close(clientSocket_) == -1)
		std::cout << "Ошибка при закрытии сокета клиента.\n";
}

void Server::start() {
	acceptClient();
}

void Server::stop() {
	if (shutdown(clientSocket_, SHUT_RDWR) == -1)
		std::cout << "Ошибка при закрытии соединения.\n";

	if (close(clientSocket_) == -1)
		std::cout << "Ошибка при закрытии сокета клиента.\n";
	
	if (close(serverSocket_) == -1)
		std::cout << "Ошибка при закрытии сокета сервера.\n";
}
