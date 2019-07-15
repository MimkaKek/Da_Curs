#include "main_help.h"
/* main_help.cpp */

bool KeyManager(std::string gotKeys) {
	for (int j = 1; j < gotKeys.size(); ++j) {
		switch (gotKeys[j]) {
			case 'c': {
				if (!keys[3] || !keys[5]) {
					keys[0] = true;
				}
				if (keys[0]) {
					keys[2] = false;
				}
				break;
			}
			case 'd': {
				if (!keys[3] || !keys[5]) {
					keys[1] = true;
				}
				if (keys[1]) {
					keys[6] = false;
					keys[7] = false;
				}
				break;
			}
			case 'k': {
				if (!keys[0] || !keys[3] || !keys[5]) {
					keys[2] = true;
				}
				break;
			}
			case 'l': {
				keys[3] = true;
				keys[0] = false;
				keys[1] = false;
				keys[2] = false;
				keys[5] = false;
				keys[6] = false;
				keys[7] = false;
				break;
			}
			case 'r': {
				keys[4] = true;
				break;
			}
			case 't': {
				if (!keys[3]) {
					keys[5] = true;
				}
				if (keys[5]) {
					keys[0] = false;
					keys[1] = false;
					keys[2] = false;
					keys[6] = false;
					keys[7] = false;
				}
				break;
			}
			case '1': {
				if (!keys[1] && !keys[3] && !keys[5]) {
					keys[6] = true;
				}
				if (keys[6]) {
					keys[7] = false;
				}
				break;
			}
			case '9': {
				if (!keys[1] && !keys[3] && !keys[5]) {
					keys[7] = true;
				}
				if (keys[7]) {
					keys[6] = false;
				}
				break;
			}
			default: {
				std::cout << "No such key '" << gotKeys[j] << "'" << std::endl << "Fuck you" << std::endl;
				return false;
			}
		}
	}
	return true;
}

void ShowResult(std::string fileName) {
	//TODO наверное зависит от Славиной работы с файлами
	return;
}

void DifferensOfSizes(std::string archiveName) {
	if (!IsArchive(archiveName)) {
		std::cout << archiveName << " is not an archive" << std::endl;
		return;
	}
	//TODO наверное зависит от Славиной работы с файлами
	//TODO делает свои дела
	return;
}

void WorkWithDirectory(std::string directoryName) {
	DIR* directory = opendir(directoryName.c_str());
	if (errno == ENOTDIR || errno == EACCES || errno == EBADF || errno == EMFILE || errno == ENOMEM || errno == ENOENT) {
		PrintErrors(directoryName);
		return;
	}
	struct dirent directoryFile = readdir(directory);
	while (directoryFile != NULL) {
		string tmp = directoryName + "/" + string(directoryFile->d_name);
		if (IsDirectory(tmp)) {
			WorkWithDirectory(tmp);
		}
		else {
			WorkWithFile(tmp);
		}
		directoryFile = readdir(directory);
	}
	closedir(directory);
	if (errno == EBADF) {
		PrintErrors(directoryName);
	}
	return;
}

void WorkWithFile(std::string fileName) {
	if (keys[3]) {//-l
		DifferensOfSizes(fileName);
		return;
	}
	if (keys[5]) {//-t
		CheckIntegrity(fileName);
		return;
	}
	if (keys[1]) {//-d
		// TODO наверное зависит от Славиной работы с файлами
		// TODO какой-нибудь Decompress который примет параметры для печати в out 
		// или сохранению обоих файлов
		return;
	}
	if (keys[6]) {//-1
		// TODO наверное зависит от Славиной работы с файлами
		// TODO какой-нибудь Compress который примет параметры для печати в out 
		// или сохранению обоих файлов и будет быстро сжимать
		return;
	}
	if (keys[7]) {//-9
		// TODO наверное зависит от Славиной работы с файлами
		// TODO какой-нибудь Compress который примет параметры для печати в out 
		// или сохранению обоих файлов и будет охрененно сжимать
		return;
	}
	if (keys[2]) {//-k
		//TODO наверное зависит от Славиной работы с файлами
		// TODO какой-нибудь Compress который примет параметры для сохранения обоих файлов
		// и будет средне сжимать
	}
	else if (keys[0]) {//-c
		// TODO наверное зависит от Славиной работы с файлами
		// TODO какой-нибудь Compress который примет параметры для печати в out 
		// и будет средне сжимать
	}
	else {//нет ключей => обычная архивация
		// TODO наверное зависит от Славиной работы с файлами
		// TODO какой-нибудь Compress который будет средне сжимать
	}
	return;
}

void CheckIntegrity(std::string archiveName) {
	if (!IsArchive(archiveName)) {
		std::cout << archiveName << " is not an archive" << std::endl;
		return;
	}
	//TODO наверное зависит от Славиной работы с файлами
	//TODO делает свои дела
	return;
}

bool IsDirectory(std::string directoryName) {
	DIR* directory = opendir(directoryName.c_str());
	if (errno == ENOTDIR || errno == EACCES || errno == EBADF || errno == EMFILE || errno == ENOMEM || errno == ENOENT) {
		PrintErrors(directoryName);
		return false;
	}
	closedir(directory);
	if (errno == EBADF) {
		PrintErrors(directoryName);
		return false;
	}
	return true;
}

void PrintErrors(std::string directoryName) {
	switch (errno) {
		case ENOTDIR: {
			std::cout << "Oh, thats not a directory" << directoryName << std::endl;
			break;
		}
		case EACCES: {
			std::cout << "No permission for directory " << directoryName << "\t Try it next time" << std::endl;
			break;
		}
		case EBADF: {
			std::cout << "Not a valid descriptor for directory " << directoryName << "\t Try it next time" << std::endl;
			break;
		}
		case EMFILE: {
			std::cout << "Too many files opened in system. Can't open directory" << directoryName
					  << "\t Try it next time" << std::endl;
			break;
		}
		case ENOMEM: {
			std::cout << "Not enought memory for opening directory " << directoryName << "\t Try it next time" << std::endl;
			break;
		}
		case ENOENT: {
			std::cout << "No directory with name " << directoryName << std::endl;
			break;
		}
	}
	return;
}

bool IsArchive(std::string fileName) {
	int size = fileName.length() - 1;
	return (fileName[size - 2] == '.' && fileName[size - 1] == 'g' && fileName[size] == 'z')
}
