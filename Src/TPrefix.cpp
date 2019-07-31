#include "TPrefix.h"

unsigned long long int TPrefix::Border;
InBinary* TPrefix::ForRead;
OutBinary* TPrefix::ForWrite;
char TPrefix::LastLetter;
unsigned long long int TPrefix::LastNumber;

TPrefix::TPrefix(unsigned long long int highBorder, InBinary* from, OutBinary* to) {
	this->Border = highBorder;
	if (highBorder == 0) {
		return;
	}
	this->ForRead = from;
	this->ForWrite = to;
	this->NumberOfWord = -1;
	this->LastLetter = 0;
	this->LastNumber = 1;
	this->Next.reserve(CHAR_HAS);
	for (int i = 0; i < CHAR_HAS; ++i) {
		this->Next[i] = {(char) i, new TPrefix()};
	}
}

TPrefix::TPrefix() {
	this->NumberOfWord = this->LastNumber;
	++this->LastNumber;
}

int TPrefix::Update(char letter) {
	bool needNew = true;
	for (int i = 0; i < this->Next.size(); ++i) {
		if (letter == this->Next[i].first) {
			needNew = false;
			if (!this->ForRead->Read(&letter, CHAR)) {
				if (!this->ForWrite->Write((char*)&this->Next[i].second->NumberOfWord, LLINT)) {
					return WRITE_ERROR;
				}
				return GOT_EOF;
			}
			int result = this->Next[i].second->Update(letter);
			if (result != OK) {
				return result;
			}
			break;
		}
	}
	if (needNew) {
		this->Next.push_back({letter, new TPrefix()});
		if (this->Next.back().second == nullptr) {
			return MEMORY_ERROR;
		}
		this->LastLetter = letter;
		if (!this->ForWrite->Write((char*)&this->NumberOfWord, LLINT)) {
			return WRITE_ERROR;
		}
	}
	return OK;
}

int TPrefix::UpdateForRoot() {
	char letter;
	bool needNew = true;
	int result;
	if (this->LastLetter == 0) {
		if (!this->ForRead->Read(&letter, CHAR)) {
			return GOT_EOF;
		}
	}
	else {
		letter = this->LastLetter;
	}
	for (int i = 0; i < this->Next.size(); ++i) {
		if (this->Next[i].first == letter) {
			needNew = false;
			if (!this->ForWrite->Write((char*)&this->Next[(int) letter].second->NumberOfWord, LLINT)) {
				return WRITE_ERROR;
			}
			break;
		}
	}
	if (needNew) {
		this->Next.push_back({letter, new TPrefix()});
		if (this->Next.back().second == nullptr) {
			return MEMORY_ERROR;
		}
		if (!this->ForWrite->Write((char*) (this->LastNumber - 1), LLINT)) {
			return WRITE_ERROR;
		}
	}
	while (true) {
		needNew = true;
		for (int i = 0; i < this->Next.size(); ++i) {
			if (letter == this->Next[i].first) {
				needNew = false;
				if (!this->ForRead->Read(&letter, CHAR)) {
					if (!this->ForWrite->Write((char*)&this->Next[i].second->NumberOfWord, LLINT)) {
						return WRITE_ERROR;
					}
					return GOT_EOF;
				}
				result = this->Next[i].second->Update(letter);
				if (result != OK) {
					return result;
				}
				break;
			}
		}
		if (needNew) {
			this->Next.push_back({letter, new TPrefix()});
			if (this->Next.back().second == nullptr) {
				return MEMORY_ERROR;
			}
			if (!this->ForWrite->Write((char*) (this->LastNumber - 1), LLINT)) {
				return WRITE_ERROR;
			}
			if (!this->ForRead->Read(&letter, CHAR)) {
				return GOT_EOF;
			}
		}
		if (this->LastNumber == this->Border) {
			if(!this->ForWrite->Write((char*) 0, LLINT)) {
				return WRITE_ERROR;
			}
			return FULL;
		}
		if (needNew) {
			if(!this->ForRead->Read(&letter, CHAR)) {
				return GOT_EOF;
			}
		}
		else {
			letter = this->LastLetter;
		}
	}
}

void TPrefix::Clear(bool root) {
	for (int i = 0; i < this->Next.size(); ++i) {
		if (root) {
			this->Next[i].second->Clear(false);
		}
		else {
			delete this->Next[i].second;
		}
	}
	if (!root) {
		this->Next.clear();
		this->Next.shrink_to_fit();
	}
	return;
}

TPrefix::~TPrefix() {
	for (int i = 0; i < this->Next.size(); ++i) {
		delete this->Next[i].second;
	}
}
