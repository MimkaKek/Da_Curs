#include "LZW.h"

TLZW::TLZW(int compressionRatio, InBinary* from, OutBinary* to) {
	this->ForRead = from;
	this->ForWrite = to;
	unsigned long long int fileSize = this->ForRead->SizeFile();
	unsigned long long int divider = std::log10(fileSize);
	switch (compressionRatio) {
		case FAST:
			if (fileSize > MINIMUM_BORDER) {
				fileSize /= pow(divider, 3);
			}
			break;
		case NORMAL:
			if (fileSize > MINIMUM_BORDER) {
				fileSize /= pow(divider, 2);
			}
			break;
		case HIGH:
			fileSize = HIGH_BORDER;
			break;
		case DECOMPRESS:
			fileSize = 0;
			break;
	}
	this->CompressionTree = new TPrefix(fileSize, from, to);
	return;
}

bool TLZW::Compress(std::string fileName) {
	char method = 'W';
	unsigned long long int size = this->ForRead->SizeFile();
	if (keys[0]) {
		std::cout << method << size;
	}
	else if (!keys[5]) {
		if (!this->ForWrite->Write(&method, CHAR)) {
			std::cout << fileName << ": can't write in file." << std::endl;
			return false;
		}
		if (!this->ForWrite->Write((char*)&size, LLINT)) {
			std::cout << fileName << ": can't write in file." << std::endl;
			return false;
		}
	}
	if (size == 0) {//TODO EOF ERROR
		return true;
	}
	int bufferState = this->CompressionTree->UpdateForRoot();
	while (bufferState == FULL) {
		this->CompressionTree->Clear(true);
		bufferState = this->CompressionTree->UpdateForRoot();
	}
	if (bufferState == MEMORY_ERROR) {
		std::cout << fileName << ": unexpected memory error" << std::endl;
		return false;
	}
	else if (bufferState == WRITE_ERROR) {
		std::cout << fileName << ": can't write in file" << std::endl;
		return false;
	}
	return true;
}


bool TLZW::Decompress(std::string fileName) {
	unsigned long long int letter, alreadyRead, wordCounter;
	std::string previousWord, presentWord;
	if (!this->ForRead->Read((char*)&wordCounter, LLINT)) {
		std::cout << fileName << ": can't read file" << std::endl;
		return false;
	}
	const unsigned long long int fileSize = wordCounter;
	if (fileSize == 0) {//TODO EOF ERROR
		return true;
	}
	std::map<unsigned long long int, std::string>::iterator finder;
	wordCounter = CHAR_HAS;
	alreadyRead = 0;
	while (this->ForRead->Read((char*)&letter, LLINT)) {
		for (int i = 0; i < CHAR_HAS; ++i) {
			this->DecompressionTree.insert({i + 1, std::string(1, (char) i)});
		}
		++alreadyRead;
		finder = this->DecompressionTree.find(letter);
		if (finder != this->DecompressionTree.end()) {
			previousWord = finder->second;
		}
		else {
			previousWord = std::string(1, (char) letter);
			this->DecompressionTree.insert({wordCounter + 1, previousWord});
			++wordCounter;
		}
		if (keys[0]) {
			std::cout << previousWord;
		}
		else if (!keys[5]) {
			if (!this->ForWrite->Write((char*)previousWord.c_str(), previousWord.size())) {
				std::cout << fileName << ": can't write in file" << std::endl;
				return false;
			}
		}
		if (alreadyRead == fileSize) {
			return true;
		}
		/*if (!this->ForRead->Read((char*)&letter, LLINT)) {
			break;
		}*/
		while (this->ForRead->Read((char*)&letter, LLINT)) {
			if (letter == 0) {
				break;
			}
			finder = this->DecompressionTree.find(letter);
			if (finder != this->DecompressionTree.end()) {
				if (keys[0]) {
					std::cout << finder->second;
				}
				else if (!keys[5]) {
					if (!this->ForWrite->Write((char*)finder->second.c_str(), finder->second.size())) {
						std::cout << fileName << ": can't write in file" << std::endl;
						return false;
					}
				}
				alreadyRead += finder->second.size();
				if (alreadyRead == fileSize) {
					return true;
				}
				presentWord = previousWord + finder->second.front();
				previousWord = finder->second;
			}
			else {
				presentWord = std::string(1, (char) letter);
				++alreadyRead;
				if (keys[0]) {
					std::cout << presentWord;
				}
				else if (!keys[5]) {
					if (!this->ForWrite->Write((char*)presentWord.c_str(), presentWord.size())) {
						std::cout << fileName << ": can't write in file" << std::endl;
						return false;
					}
				}
				if (alreadyRead == fileSize) {
					return true;
				}
				previousWord = presentWord;
			}
			this->DecompressionTree.insert({wordCounter + 1, presentWord});
			++wordCounter;
		}
		this->DecompressionTree.clear();
		if (letter != 0) {
			break;
		}
	}
	if (alreadyRead < fileSize) {
		std::cout << fileName << ": unexpected end of file" << std::endl;
		return false;
	}
	return true;
}

TLZW::~TLZW() {
	delete this->CompressionTree;
}
