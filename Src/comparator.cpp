#include <iostream>
#include <vector>
#include "BFile.h"

//программа будет заниматься сравнением данных *до* и *после* архивации
//Может быть когда-нибудь где-нибудь зачем-нибудь ещё пригодится

void Compare(InBinary* original, InBinary* afterWork, int number) {
	unsigned long long int origSize, afterSize;
	char origLetter, afterLetter;
	const int CHARS = sizeof(char);
	origSize = original->SizeFile();
	afterSize = afterWork->SizeFile();
	/*if (origSize != afterSize) {//временно закоментил для поиска бага в LZW
		std::cout << number << ": different sizes" << std::endl;
		return;
	}*/
	for (int i = 0; i < origSize; ++i) {
		if (!original->Read(&origLetter, CHARS)) {
			std::cout << number << ": orig bad read" << std::endl;
			return;
		}
		if (!afterWork->Read(&afterLetter, CHARS)) {
			std::cout << number << ": after bad read" << std::endl;
			return;
		}
		if (origLetter != afterLetter) {
			std::cout << number << ": bad compare: letter " << i << std::endl;
			return;
		}
	}
	return;
}

int main() {
	std::vector<std::pair<std::string, std::string>> files = {
		{"test01", "test02"},
		{"test11", "test12"},
		{"test21", "test22"},
		{"test31", "test32"},
		{"test41", "test42"},
		{"test51", "test52"}
	};
	InBinary* original	= 	new InBinary;
	InBinary* afterWork	=	new InBinary;
	for (int i = 0; i < files.size(); ++i) {
		if (!original->Open(&files[i].first)) {
			std::cout << "no file " << files[i].first << std::endl;
			continue;
		}
		if (!afterWork->Open(&files[i].second)) {
			std::cout << "no file " << files[i].second << std::endl;
			continue;
		}
		Compare(original, afterWork, i);
		original->Close();
		afterWork->Close();
	}
	delete original;
	delete afterWork;
	return 0;
}
