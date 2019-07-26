#include "TPrefix.h"

//TODO GetLetter()
//TODO PushInFile()
//TODO MEMORY

TPrefix::TPrefix(unsigned long long int highBorder) {
	this->border = highBorder;
	if (highBorder == 0) {
		return;
	}
	this->numberOfWord = -1;
	this->lastLetter = 0;
	this->lastNumber = 1;
	this->next.reserve(ASCII_SIMBOLS);
	for (int i = 0; i < CHAR_HAS; ++i) {
		this->next[i] = {(unsigned char) i, new TPrefix()};
	}
	return;
}

TPrefix::TPrefix() {
	this->numberOfWord = this->lastNumber;
	++this->lastNumber;
}

bool TPrefix::Update(unsigned char letter) {
	bool needNew = true;
	for (int i = 0; i < this->next.size(); ++i) {
		if (letter == this->next[i].first) {
			needNew = false;
			letter = GetLetter();
			this->next[i].second->Update(letter);
			break;
		}
	}
	if (needNew) {
		this->next.push_back({letter, new TPrefix()});
		this->lastLetter = letter;
		PushInFile(this->numberOfWord);
	}
	return true;
}

int TPrefix::UpdateForRoot() {
	unsigned char letter;
	if (this->lastLetter == 0) {
		letter = GetLetter();
	}
	else {
		letter = this->lastLetter;
	}
	bool needNew = true;
	if (letter != EOF) {
		for (int i = 0; i < this->next.size(); ++i) {
			if (this->next[i].first == letter) {
				needNew = false;
				PushInFile(this->next[(int) letter].second->numberOfWord);
				break;
			}
		}
		if (needNew) {
			this->next.push_back({letter, new TPrefix()});
			PushInFile(this->lastNumber - 1);
		}
	}
	while (letter != EOF) {
		needNew = true;
		for (int i = 0; i < this->next.size(); ++i) {
			if (letter == this->next[i].first) {
				needNew = false;
				letter = GetLetter();
				this->next[i].second->Update(letter);
				break;
			}
		}
		if (needNew) {
			this->next.push_back({letter, new TPrefix()});
			PushInFile(this->next.back()->numberOfWord);
			letter = GetLetter();
		}
		else {
			letter = this->lastLetter;
		}
		if (this->lastNumber == this->border) {//буфер заполнен
			PushInFile(0);
			return 1;
		}
	}
	return 0;
}

void TPrefix::Clear(bool root) {
	for (int i = 0; i < this->next.size(); ++i) {
		if (root) {
			this->next[i].Clear(false);
		}
		else {
			delete this->next[i].second;
		}
	}
	if (!root) {
		this->next.clear();
		this->next.shrink_to_fit();
	}
	return;
}

TPrefix::~TPrefix() {
	for (int i = 0; i < this->next.size(); ++i) {
		delete this->next[i].second;
	}
}
