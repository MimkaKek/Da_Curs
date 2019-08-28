/* LZW.cpp */
#include "LZW.h"

TLZW::TLZW(int compressionRatio, TInBinary* from, TOutBinary* to) {
	this->ForRead = from;
	this->ForWrite = to;
	unsigned long long int fileSize = this->ForRead->SizeFile();
	if (fileSize > MINIMUM_BORDER) {
		unsigned long long int divider = std::log10(fileSize) + 1;
		switch (compressionRatio) {
			case FAST:
				fileSize /= pow(divider, 3);
				fileSize += CHAR_HAS;
				break;
			case NORMAL:
				fileSize /= pow(divider, 2);
				fileSize += CHAR_HAS;
				break;
			case HIGH:
				fileSize = this->DecompressionTree.max_size();
				break;
		}
	}
	else {
		fileSize = CHAR_HAS * 2;
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
		if (!this->ForWrite->Write(&method, sizeof(char))) {
			std::cout << std::endl << "\t\t" << fileName << ": can't write in file." << std::endl;
			return false;
		}
		if (!this->ForWrite->Write((char*)&size, sizeof(unsigned long long int))) {
			std::cout << std::endl << "\t\t" << fileName << ": can't write in file." << std::endl;
			return false;
		}
	}
	if (size == 0) {
		return true;
	}
	int bufferState = this->CompressionTree->UpdateForRoot();
	while (bufferState == FULL) {
		this->CompressionTree->Clear(true);
		bufferState = this->CompressionTree->UpdateForRoot();
	}
	if (bufferState == MEMORY_ERROR) {
		std::cout << std::endl << "\t\t" << fileName << ": unexpected memory error" << std::endl;
		return false;
	}
	else if (bufferState == WRITE_ERROR) {
		std::cout << std::endl << "\t\t" << fileName << ": can't write in file" << std::endl;
		return false;
	}
	else if (bufferState == READ_ERROR) {
		std::cout << std::endl << "\t\t" << fileName << ": bad input" << std::endl;
		return false;
	}
	return true;
}


bool TLZW::Decompress(std::string fileName) {
	unsigned long long int letter;
	std::string previousWord, presentWord;
	unsigned short int bites;
	if (!this->ForRead->Read((char*)&letter, sizeof(unsigned long long int))) {
		std::cout << fileName << ": can't read file" << std::endl;
		return false;
	}
	if (letter < std::numeric_limits<unsigned short int>::max()) {
		bites = sizeof(short int);
	}
	else if (letter < std::numeric_limits<unsigned int>::max()) {
		bites = sizeof(int);
	}
	else {
		bites = sizeof(long long int);
	}
	const unsigned long long int fileSize = letter;
	if (fileSize == 0) {
		return true;
	}
	std::map<unsigned long long int, std::string>::iterator finder;
	unsigned long long int wordCounter = CHAR_HAS;
	unsigned long long int alreadyRead = 0;
	while (this->ForRead->Read((char*)&letter, bites)) {
		for (int i = 0; i < CHAR_HAS; ++i) {
			this->DecompressionTree.insert({i + 1, std::string(1, (char) i)});
		}
		++alreadyRead;
		finder = this->DecompressionTree.find(letter);
		previousWord = finder->second;
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
		while (this->ForRead->Read((char*)&letter, bites)) {
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
			else if (letter == wordCounter + 1) {
				presentWord = previousWord + previousWord.front();
				alreadyRead += presentWord.size();
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
			else {
				std::cout << fileName << ": invalid compressed data" << std::endl;
				return false;
			}
			this->DecompressionTree.insert({wordCounter + 1, presentWord});
			++wordCounter;
		}
		this->DecompressionTree.clear();
		if (letter != 0) {
			letter = 1;
			break;
		}
		wordCounter = CHAR_HAS;
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
