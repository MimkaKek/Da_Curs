#include "main_help.h"
/* main_help.cpp */

bool KeyManager(std::string gotKeys) {
	for (int j = 1; j < gotKeys.size(); ++j) {
		switch (gotKeys[j]) {
			case 'c':
				if (!keys[3] || !keys[5]) {
					keys[0] = true;
				}
				if (keys[0]) {
					keys[2] = false;
				}
				break;
			case 'd':
				if (!keys[3] || !keys[5]) {
					keys[1] = true;
				}
				if (keys[1]) {
					keys[6] = false;
					keys[7] = false;
				}
				break;
			case 'k':
				if (!keys[0] || !keys[3] || !keys[5]) {
					keys[2] = true;
				}
				break;
			case 'l':
				keys[3] = true;
				keys[0] = false;
				keys[1] = false;
				keys[2] = false;
				keys[5] = false;
				keys[6] = false;
				keys[7] = false;
				break;
			case 'r':
				keys[4] = true;
				break;
			case 't':
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
			case '1':
				if (!keys[1] && !keys[3] && !keys[5]) {
					keys[6] = true;
				}
				if (keys[6]) {
					keys[7] = false;
				}
				break;
			case '9':
				if (!keys[1] && !keys[3] && !keys[5]) {
					keys[7] = true;
				}
				if (keys[7]) {
					keys[6] = false;
				}
				break;
			default:
				std::cout << "No such key '" << gotKeys[j] << "'" << std::endl << "Fuck you" << std::endl;
				return false;
		}
	}
	return true;
}

void ShowResult(std::string fileName) {//TODO
	
	return;
}

void DifferensOfSizes(std::string archiveName) {//TODO
	if (!IsArchive(archiveName)) {
		std::cout << archiveName << " is not an archive" << std::endl;
		return;
	}
	//TODO делает свои дела
	return;
}

void WorkWithDirectory(std::string directoryName) {
	DIR* directory = opendir(directoryName.c_str());
	if (errno == ENOTDIR || errno == EACCES || errno == EBADF || errno == EMFILE || errno == ENOMEM || errno == ENOENT) {
		PrintErrors(directoryName);
		return;
	}
	struct dirent* directoryFile = readdir(directory);
	while (directoryFile != NULL) {
		std::string tmp = directoryName + std::string(directoryFile->d_name);
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

void WorkWithFile(std::string fileName) {//TODO
	if (keys[3]) {//-l
		DifferensOfSizes(fileName);
		std::cout << "info" << std::endl;//TEST
	}
	else if (keys[5]) {//-t
		CheckIntegrity(fileName);
		std::cout << "integrity" << std::endl;//TEST
	}
	else if (keys[1]) {//-d
		//TODO
		/*FILE* tmp = Decompress(fileName);
		std::string tmpName = tmp->name;
		if (keys[0]) {//-c
			ShowResult(tmpName);
			Delete(tmpName);
			return;
		}
		Save(tmpName);
		if (!keys[2]) {//-k
			Delete(fileName);
		}*/
		std::cout << "decompress" << std::endl;//TEST
	}
	else if (keys[6]) {//-1
		//TODO
		/*FILE* tmp = Compress(fileName, FAST);
		std::string tmpName = tmp->name;
		if (keys[0]) {//-c
			ShowResult(tmpName);
			Delete(tmpName);
			return;
		}
		Save(tmpName);
		if (!keys[2]) {//-k
			Delete(fileName);
		}*/
		std::cout << FAST << std::endl;//TEST
	}
	else if (keys[7]) {//-9
		//TODO
		/*FILE* tmp = Compress(fileName, HIGH);
		std::string tmpName = tmp->name;
		if (keys[0]) {//-c
			ShowResult(tmpName);
			Delete(tmpName);
			return;
		}
		Save(tmpName);
		if (!keys[2]) {//-k
			Delete(fileName);
		}*/
		std::cout << HIGH << std::endl;//TEST
	}
	else if (keys[2]) {//-k
		//TODO
		/*FILE* tmp = Compress(fileName, NORMAL);
		std::string tmpName = tmp->name;
		Save(tmpName);*/
		std::cout << NORMAL << "\t-k" << std::endl;//TEST
	}
	else if (keys[0]) {//-c
		//TODO
		/*FILE* tmp = Compress(fileName, NORMAL);
		std::string tmpName = tmp->name;
		ShowResult(tmpName);
		Delete(tmpName);*/
		std::cout << NORMAL << "\t-c" << std::endl;//TEST
	}
	else {//нет ключей => обычная архивация
		//TODO
		/*FILE* tmp = Compress(fileName, NORMAL);
		std::string tmpName = tmp->name;
		Save(tmpName);
		Delete(fileName);*/
		std::cout << NORMAL << " NORMAL" << std::endl;//TEST
	}
	return;
}

void CheckIntegrity(std::string archiveName) { //TODO
	if (!IsArchive(archiveName)) {
		std::cout << archiveName << " is not an archive" << std::endl;
		return;
	}
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
		case ENOTDIR://TEST
			std::cout << "Oh, thats not a directory\t" << directoryName << std::endl;
			break;
		case EACCES:
			std::cout << "No permission for directory " << directoryName << "\t Try it next time" << std::endl;
			break;
		case EBADF:
			std::cout << "Not a valid descriptor for directory " << directoryName << "\t Try it next time" << std::endl;
			break;
		case EMFILE:
			std::cout << "Too many files opened in system. Can't open directory" << directoryName
					  << "\t Try it next time" << std::endl;
			break;
		case ENOMEM:
			std::cout << "Not enought memory for opening directory " << directoryName << "\t Try it next time" << std::endl;
			break;
		/*case ENOENT: TODO перенести этот кейс в другое место и добавить файлы
			std::cout << "No directory with name " << directoryName << std::endl;
			break;*/
	}
	return;
}

bool IsArchive(std::string fileName) {
	int size = fileName.length() - 1;
	return (fileName[size - 2] == '.' && fileName[size - 1] == 'g' && fileName[size] == 'z');
}

void Save(std::string fileName) {//TODO
	//TODO найти в той же дирректории файл с таким же именем
	if (true/*нашел*/) {
		std::cout << fileName << " already exists; do you wish to overwrite (y or n)? ";
		char answer;
		std::cin >> answer;
		if (answer == 'y') {
			//TODO
		}
		else {
			std::cout << "\tnot overwritten" << std::endl;
		}
	}
	return;
}

void Delete(std::string fileName) {
	std::string command = "rm " + fileName;
	system(command.c_str());
	return;
}
