#include "BufferManager.h"

void BufferManager::setString(const std::string& input) {
	std::unique_lock<std::mutex> lock(bufferMutex_);
	
	buffer_ = input;
	
	charCount_.clear();
	for (const char& ch : buffer_)
		charCount_[ch]++;
	
	bufferCondVar_.notify_one();
}

std::map<char, unsigned> BufferManager::getProcessedData() {
	std::unique_lock<std::mutex> lock(bufferMutex_);
	return charCount_;
}

void BufferManager::clearBuffer() {
	std::unique_lock<std::mutex> lock(bufferMutex_);
	buffer_.clear();
	charCount_.clear();
}

bool BufferManager::isEmpty() {
	return (buffer_.size() == 0 && charCount_.size() == 0);
}

void BufferManager::waitForData() {
	std::unique_lock<std::mutex> lock(bufferMutex_);
	bufferCondVar_.wait(lock, [this] { return !isEmpty(); });
}
