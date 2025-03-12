#pragma once

#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <condition_variable>

class BufferManager {
public:
	void setString(const std::string&);
	std::map<char, unsigned> getProcessedData();
	
	void clearBuffer();
	bool isEmpty();
	
	void waitForData();
private:
	std::string buffer_;
	std::map<char, unsigned> charCount_;
	
	std::mutex bufferMutex_;
	std::condition_variable bufferCondVar_;
};
