#include "LZW.h"

TLZW::TLZW(int compressionRatio, InBinary* from, OutBinary* to) {
	this->ForRead = from;
	this->ForWrite = to;
	unsigned long long int fileSize = this->ForRead->SizeFile(); // TODO SLAVA
	if (compressionRatio == DECOMPRESS) {
		fileSize = 0;
	}
	else if (fileSize < LOW_BORDER && fileSize > 3) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= 3;
				break;
			case NORMAL:
				fileSize *= (2.0 / 3.0);
				break;
		}
	}
	else if (fileSize >= LOW_BORDER && fileSize < MEDIUM_BORDER) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= LOW_FAST;
				break;
			case NORMAL:
				fileSize /= LOW_MEDIUM;
				break;
		}
	}
	else if (fileSize >= MEDIUM_BORDER && fileSize < HIGH_BORDER) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= MEDIUM_FAST;
				break;
			case NORMAL:
				fileSize /= MEDIUM_MEDIUM;
				break;
		}
	}
	else if (fileSize >= HIGH_BORDER) {
		switch (compressionRatio) {
			case FAST:
				fileSize = HIGH_BORDER / HIGH_FAST;
				break;
			case NORMAL:
				fileSize = HIGH_BORDER / HIGH_MEDIUM;
				break;
			case HIGH:
				fileSize = HIGH_BORDER;
				break;
		}
	}
	this->CompressionTree = new TPrefix(fileSize, from, to);
	return;
}

bool TLZW::Compress(std::string fileName) {
	char method = 'W';
	unsigned long long int size = this->ForRead->SizeFile();
	if (!this->ForWrite->Write(&method, sizeof(char))) {
		return false;
	}
	if (!this->ForWrite->Write((char*)&size, sizeof(unsigned long long int))) {
		return false;
	}
	int bufferState = this->CompressionTree->UpdateForRoot();
	while (bufferState == FULL) {
		this->CompressionTree->Clear(true);
		bufferState = this->CompressionTree->UpdateForRoot();
	}
	if (bufferState == MEMORY_ERROR) {
		return false;
	}
	return true;
}

bool TLZW::Decompress() {
	unsigned long long int letter, alreadyRead, wordCounter;
	std::string previousWord, presentWord;
	const unsigned long long int fileSize = GetSize();
	std::map<unsigned long long int, std::string>::iterator finder;
	wordCounter = CHAR_HAS;
	letter = GetLetter();
	while (letter != EOF) {
		for (int i = 0; i < CHAR_HAS; ++i) {
			this->DecompressionTree.insert({i + 1, std::string(1, (unsigned char) i)});
		}
		++alreadyRead;
		finder = this->DecompressionTree.find(letter);
		if (finder != this->DecompressionTree.end()) {
			previousWord = finder.second;
		}
		else {
			previousWord = std::string(1, (unsigned char) letter);
			this->DecompressionTree.insert({wordCounter + 1, previousWord});
			++wordCounter;
		}
		PushInFile(previousWord);
		letter = GetLetter();
		while (letter != 0 && letter != EOF) {
			++alreadyRead;
			finder = this->DecompressionTree.find(letter);
			if (finder != this->DecompressionTree.end()) {
				PushInFile(finder.second);
				presentWord = previousWord + finder.second.front();
				previousWord = finder.second;
			}
			else {
				presentWord = std::string(1, (unsigned char) letter);
				PushInFile(presentWord);
				previousWord = presentWord;
			}
			this->DecompressionTree.insert({wordCounter + 1, presentWord});
			++wordCounter;
			letter = GetLetter();
		}
		this->DecompressionTree.clear();
		if (letter == 0) {
			letter = GetLetter();
		}
	}
	if (alreadyRead != fileSize) {
		return false;
	}
	return true;
}

TLZW::~TLZW() {
	delete this->CompressionTree;
}
