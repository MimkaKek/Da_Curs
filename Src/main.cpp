#include "main_help.h"

std::vector<bool> keys;

int main(int argc, char *argv[]) {
	std::vector<std::string> fileNames;
	keys = {false, false, false, false, false, false, false, false};
//			c	   d	  k		 l		r	   t	  1		 9
	for (int i = 1; i < argc; ++i) {
		std::string keyFile;
		std::stringstream tmp(argv[i]);
		tmp >> keyFile;
		if (keyFile[0] == '-') {//ага ключ(и)
			if (!KeyManager(keyFile)) {
				return -1;
			}
		}
		else {// ага файлы и папки
			bool got = false;
			for (int j = 0; j < fileNames.size(); ++j) {
				if (fileNames[j] == keyFile) {//для избежания работы над одним и тем же файлом несколько раз
					got = true;
					break;
				}
			}
			if (!got) {
				fileNames.push_back(keyFile);
			}
		}
	}
	if (fileNames.empty()) {
		std::cout << "Compressed data not written to a terminal." << std::endl;
		return 0;
	}
	for (int i = 0; i < fileNames.size(); i++) {//проверка на файлы и папки и дальнейшая работа с ними
		bool directory = IsDirectory(fileNames[i], false);
		if (!keys[4] && directory) {
			std::cout << fileNames[i] << " is a directory -- ignored" << std::endl;
		}
		else if (directory) {
			WorkWithDirectory(fileNames[i]);
		}
		else {
			if (errno == ENOTDIR) {
				WorkWithFile(fileNames[i]);
			}
			else {
				PrintDirectoryErrors(fileNames[i]);
			}
		}
	}
	//TEST
	int ti = clock();
	//std::cout << ((double)ti) / CLOCKS_PER_SEC << " секунд" << std::endl;
	return 0;
}
