#include "TPrefix.h"

TPrefix::TPrefix(char borderSignal, unsigned int highBorder) {
	this->border = highBorder;
	if (borderSignal == 'd') {
		return;
	}
	this->numberOfWord = -1;
	this->lastNumber = 0;
	this->next(ASCII_SIMBOLS);
	for (int i = 0; i < ASCII_SIMBOLS; ++i) {
		this->next[i] = {(char) i, new TPrefix()};
	}
	return;
}

TPrefix::TPrefix() {
	this->numberOfWord = this->lastNumber;
	++this->lastNumber;
}

void TPrefix::Update(char letter) {
	
}

TPrefix::~TPrefix() {
	for (int i = 0; i < this->next.size(); ++i) {
		delete this->next[i];
	}
	return;
}
