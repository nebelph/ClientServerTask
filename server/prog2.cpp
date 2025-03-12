#include "Server.h"

#include <iostream>

int main(){
	while (true) {
		try {
			Server s;
			s.start();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	
	return 0;
}
