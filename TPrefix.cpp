#include "TPrefix.h"

//TODO GetLetter()
//TODO PushInFile()
//TODO MEMORY

TPrefix::TPrefix(unsigned long long int highBorder, InBinary* from, OutBinary* to) {
	this->Border = highBorder;
	if (highBorder == 0) {
		return;
	}
	this->read = from;
	this->write = to;
	this->NumberOfWord = -1;
	this->LastLetter = 0;
	this->LastNumber = 1;
	this->Next.reserve(ASCII_SIMBOLS);
	for (int i = 0; i < CHAR_HAS; ++i) {
		this->Next[i] = {(unsigned char) i, new TPrefix()};
	}
	return;
}

TPrefix::TPrefix() {
	this->NumberOfWord = this->LastNumber;
	++this->LastNumber;
}

bool TPrefix::Update(unsigned char letter) {
	bool needNew = true;
	for (int i = 0; i < this->Next.size(); ++i) {
		if (letter == this->Next[i].first) {
			needNew = false;
			letter = GetLetter();
			this->Next[i].second->Update(letter);
			break;
		}
	}
	if (needNew) {
		this->Next.push_back({letter, new TPrefix()});
		this->LastLetter = letter;
		PushInFile(this->NumberOfWord);
	}
	return true;
}

int TPrefix::UpdateForRoot() {
	unsigned char letter;
	bool needNew = true;
	if (this->ForRead->Read(&letter, sizeof(char))) {
		for (int i = 0; i < this->Next.size(); ++i) {
			if (this->Next[i].first == letter) {
				needNew = false;
				if (!this->ForWrite->Write((char*)&this->Next[(int) letter].second->NumberOfWord, sizeof(unsigned long long int))) {
					return false;
				}
				break;
			}
		}
		if (needNew) {
			this->Next.push_back({letter, new TPrefix()});
			if (!this->ForWrite->Write((char*)&(this->LastNumber - 1), sizeof(unsigned long long int)) {
				return false;
			}
		}
	}
	else {
		return true;
	}
	while (this->ForRead->Read(&letter, sizeof(char))) {
		needNew = true;
		for (int i = 0; i < this->Next.size(); ++i) {
			if (letter == this->Next[i].first) {
				needNew = false;
				if (!this->ForRead->Read(&letter, sizeof(char))) {
					if (!this->ForWrite->Write((char*)&this->Next[i]->second->NumberOfWord)) {
						return false;
					}
					return true;
				}
				this->Next[i].second->Update(letter);
				break;
			}
		}
		if (needNew) {
			this->Next.push_back({letter, new TPrefix()});
			PushInFile(this->Next.back()->NumberOfWord);
			letter = GetLetter();
		}
		else {
			letter = this->LastLetter;
		}
		if (this->LastNumber == this->Border) {//буфер заполнен
			PushInFile(0);
			return 1;
		}
	}
	return 0;
}

void TPrefix::Clear(bool root) {
	for (int i = 0; i < this->Next.size(); ++i) {
		if (root) {
			this->Next[i].Clear(false);
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
