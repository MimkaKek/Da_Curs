#include "main_help.h"

int main(int argc, char *argv[]) {
	std::vector<std::string> fileNames;
	std::vector<bool> keys = {false, false, false, false, false, false, false, false};
//							  c		 d		k	   l	  r		 t		1	   9
	for (int i = 1; i < argc; ++i) {
		std::string keyFile;
		std::stringstream tmp(argv[i]);
		tmp >> keyFile;
		if (keyFile[0] == '-') {//ага ключ(и)
			if (!KeyManager(keyFile, &keys)) {
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
		bool directory = IsDirectory(fileNames[i]);
		if (!keys[4] && directory) {
			std::cout << fileNames[i] << " is a directory -- ignored" << std::endl;
		}
		else if (directory) {
			WorkWithDirectory(fileNames[i], keys);
		}
		else {
			WorkWithFile(fileNames[i], keys);
		}
	}
	for (int i = 0; i < 8; ++i) {//TEST
		std::cout << (keys[i] ? "+" : "-") << std::endl;
	}
	return 0;
}
