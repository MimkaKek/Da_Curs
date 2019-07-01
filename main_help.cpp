#include "main_help.h"
/* main_help.cpp */

void ShowArc(std::string archiveName) {
	
	return;
}

void DifferensOfSizes(std::string archiveName) {
	if (!IsArchive(archiveName)) {
		std::cout << archiveName << " is not an archive" << std::endl;
		return;
	}
	//TODO делает свои дела
	return;
}

void WorkWithDirectory(std::string directoryName, std::vector<bool> keys) {
	
	return;
}

void WorkWithFile(std::string fileName, std::vector<bool> keys) {
	if (keys[3]) {//-l
		DifferensOfSizes(fileName);
		return;
	}
	if (keys[5]) {//-t
		CheckIntegrity(fileName);
		return;
	}
	if (keys[1]) {//-d
		// TODO какой-нибудь Decompress который примет параметры для печати в out 
		// или сохранению обоих файлов
		return;
	}
	if (keys[6]) {//-1
		// TODO какой-нибудь Compress который примет параметры для печати в out 
		// или сохранению обоих файлов и будет быстро сжимать
		return;
	}
	if (keys[7]) {//-9
		// TODO какой-нибудь Compress который примет параметры для печати в out 
		// или сохранению обоих файлов и будет охрененно сжимать
		return;
	}
	if (keys[2]) {//-k
		// TODO какой-нибудь Compress который примет параметры для сохранения обоих файлов
		// и будет средне сжимать
	}
	else if (keys[0]) {//-c
		// TODO какой-нибудь Compress который примет параметры для печати в out 
		// и будет средне сжимать
	}
	else {//нет ключей => обычная архивация
		// TODO какой-нибудь Compress который будет средне сжимать
	}
	return;
}

void CheckIntegrity(std::string archiveName) {
	if (!IsArchive(archiveName)) {
		std::cout << archiveName << " is not an archive" << std::endl;
		return;
	}
	//TODO делает свои дела
	return;
}

bool IsDirectory(std::string directoryName) {
	DIR* directory = opendir(directoryName.c_str());
	if (errno == ENOTDIR) {
		std::cout << "Oh, thats not a directory" << std::endl;
		return false;
	}//TODO хм... наверно надо что-то юзать для записи ошибок
	else if (errno == EACCES || errno == EBADF || errno == EMFILE || errno == ENOENT || errno == ENOMEM) {
		std::cout << "Troubles with directory " << directoryName << "\t Try it next time" << std::endl;
		return false;
	}
	closedir(directory);
	return true;
}

bool IsArchive(std::string fileName) {
	
	return true;
}
