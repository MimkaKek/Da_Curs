#include "TPrefix.h"

unsigned long long int TPrefix::Border;
InBinary* TPrefix::ForRead;
OutBinary* TPrefix::ForWrite;
char TPrefix::LastLetter;
unsigned long long int TPrefix::LastNumber;
unsigned long long int TPrefix::NeedToRead;
unsigned short int TPrefix::Bites;

TPrefix::TPrefix(unsigned long long int highBorder, InBinary* from, OutBinary* to) {
	this->Border = highBorder;
	if (highBorder == 0) {
		return;
	}
	this->ForRead = from;
	this->ForWrite = to;
	highBorder = this->ForRead->SizeFile();
	this->NeedToRead = highBorder + 1;
	if (highBorder < std::numeric_limits<unsigned short int>::max()) {
		this->Bites = sizeof(unsigned short int);
	}
	else if (highBorder < std::numeric_limits<unsigned int>::max()) {
		this->Bites = sizeof(unsigned int);
	}
	else {
		this->Bites = sizeof(unsigned long long int);
	}
	this->NumberOfWord = 0;
	this->LastLetter = 0;
	this->LastNumber = 1;
	for (int i = 0; i < CHAR_HAS; ++i) {
		this->Next.push_back({(char) i, new TPrefix()});
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
			if (this->LastNumber < this->Border) {
				if (!this->ForRead->Read(&letter, sizeof(char))) {
					if (this->NeedToRead != 0) {
						return READ_ERROR;
					}
					if (keys[0]) {
						std::cout << (char*)&this->Next[i].second->NumberOfWord;
					}
					else if (!keys[5]) {
						if (!this->ForWrite->Write((char*)&this->Next[i].second->NumberOfWord, this->Bites)) {
							return WRITE_ERROR;
						}
					}
					return GOT_EOF;
				}
				--this->NeedToRead;
				return this->Next[i].second->Update(letter);
			}
			else {
				if (keys[0]) {
					std::cout << (char*)&this->Next[i].second->NumberOfWord;
				}
				else if (!keys[5]) {
					if (!this->ForWrite->Write((char*)&this->Next[i].second->NumberOfWord, this->Bites)) {
						return WRITE_ERROR;
					}
				}
				return FULL;
			}
		}
	}
	if (needNew) {
		this->Next.push_back({letter, new TPrefix()});
		if (this->Next.back().second == nullptr) {
			return MEMORY_ERROR;
		}
		this->LastLetter = letter;
		if (keys[0]) {
			std::cout << (char*)&this->NumberOfWord;
		}
		else if (!keys[5]) {
			if (!this->ForWrite->Write((char*)&this->NumberOfWord, this->Bites)) {
				return WRITE_ERROR;
			}
		}
	}
	return OK;
}

int TPrefix::UpdateForRoot() {
	this->LastNumber = CHAR_HAS + 1;
	char letter;
	unsigned long long int tmpInt = 0;
	if (!this->ForRead->Read(&letter, sizeof(char))) {
		if (this->NeedToRead != 0) {
			return READ_ERROR;
		}
		return GOT_EOF;
	}
	--this->NeedToRead;
	while (true) {
		for (int i = 0; i < this->Next.size(); ++i) {
			if (this->Next[i].first == letter) {
				if (this->LastNumber < this->Border) {
					if (!this->ForRead->Read(&letter, sizeof(char))) {
						if (this->NeedToRead != 0) {
							return READ_ERROR;
						}
						return GOT_EOF;
					}
					--this->NeedToRead;
					tmpInt = this->Next[i].second->Update(letter);
					if (tmpInt != OK) {
						return tmpInt;
					}
					letter = this->LastLetter;
					break;
				}
				else {
					tmpInt = 0;
					if (keys[0]) {
						std::cout << (char*)&this->Next[i].second->NumberOfWord << (char*)&tmpInt;
					}
					else if (!keys[5]) {
						if (!this->ForWrite->Write((char*)&this->Next[i].second->NumberOfWord, this->Bites)) {
							return WRITE_ERROR;
						}
						if (!this->ForWrite->Write((char*)&tmpInt, this->Bites)) {
							return WRITE_ERROR;
						}
					}
					return FULL;
				}
			}
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
