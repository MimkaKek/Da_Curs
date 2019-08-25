#include <iostream>
#include <vector>
#include "BFile.h"

//программа будет заниматься сравнением данных *до* и *после* архивации
//Может быть когда-нибудь где-нибудь зачем-нибудь ещё пригодится

bool Compare(TInBinary* original, TInBinary* afterWork, int number) {
	unsigned long long int origSize, afterSize;
	char origLetter, afterLetter;
	const int CHARS = sizeof(char);
	origSize = original->SizeFile();
	afterSize = afterWork->SizeFile();
	if (origSize != afterSize) {
		std::cout << number << ": different sizes" << std::endl;
		return false;
	}
	for (int i = 0; i < origSize; ++i) {
		if (!original->Read(&origLetter, CHARS)) {
			std::cout << number << ": orig bad read" << std::endl;
			return false;
		}
		if (!afterWork->Read(&afterLetter, CHARS)) {
			std::cout << number << ": after bad read" << std::endl;
			return false;
		}
		if (origLetter != afterLetter) {
			std::cout << number << ": bad compare: letter " << i << std::endl;
			return false;
		}
	}
	return true;
}

int main() {
	bool ok = true;
	std::vector<std::pair<std::string, std::string>> files = {
		{"test01", "test02"},
		{"test11", "test12"},
		{"test21", "test22"},
		{"test31", "test32"},
		{"test41", "test42"},
		{"test51", "test52"}
	};
	TInBinary* original	= 	new TInBinary;
	TInBinary* afterWork	=	new TInBinary;
	for (int i = 0; i < files.size(); ++i) {
		if (!original->Open(&files[i].first)) {
			std::cout << "no file " << files[i].first << std::endl;
			continue;
		}
		if (!afterWork->Open(&files[i].second)) {
			std::cout << "no file " << files[i].second << std::endl;
			continue;
		}
		if (!Compare(original, afterWork, i)) {
			ok = false;
		}
		original->Close();
		afterWork->Close();
	}
	delete original;
	delete afterWork;
	if (!ok) {
		return 1;
	}
	return 0;
}
