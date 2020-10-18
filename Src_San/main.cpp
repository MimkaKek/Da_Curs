/* maip.cpp */
#include "main_help.h"

std::vector<bool> keys;

int main(int argc, char *argv[]) {
	clock_t t0 = clock();
	std::map<std::string, int> fileNames, directoryNames;
	std::map<std::string, int>::iterator finder;
	keys = {false, false, false, false, false, false, false, false, false};
//			c	   d	  k		 l		r	   t	  1		 9		a
	for (int i = 1; i < argc; ++i) {
		std::string keyFile;
		std::stringstream tmp(argv[i]);
		tmp >> keyFile;
		if (keyFile[0] == '-') {//ключ(и)
			if (!KeyManager(keyFile)) {
				return -1;
			}
		}
		else {// файлы и папки
			if (keyFile.size() > 2) {
				if (keyFile[0] == '.' && keyFile[1] == '/') {
					keyFile.erase(0, 2);
				}
			}
			if (IsDirectory(keyFile, false)) {
				WorkWithDirectory(keyFile, *directoryNames);
			}
			else {
				bool got = false;
				finder = fileNames.find(keyFile);
				if (finder != fileNames.end()) {//для избежания работы над одним и тем же файлом несколько раз
					got = true;
				}
				if (!got) {
					fileNames.insert({keyFile, i});
				}
			}
		}
	}
	if (fileNames.empty() && directoryNames.empty()) {
		std::cout << "Compressed data not written to a terminal." << std::endl;
		return 0;
	}
	for (finder = directoryNames.begin(); finder != directoryNames.end() && keys[4]; ++finder) {
		if (finder->first == "main") {
			continue;
		}
		else {
			WorkWithFile(finder->first);
		}
	}
	for (finder = fileNames.begin(); finder != fileNames.end(); ++finder) {
		if (finder->first == "main") {
			continue;
		}
		else {
			WorkWithFile(finder->first);
		}
	}
	clock_t t1 = clock();//TODO
	if (keys[8]) {
		std::cout << std::endl << ((double) t1 - t0) / CLOCKS_PER_SEC << " секунд" << std::endl;
	}
	return 0;
}
