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
				std::cout << "invalid option -- '" << gotKeys[j] << "'" << std::endl;
				return false;
		}
	}
	return true;
}

bool DifferensOfSizes(InBinary* file, std::string fileName) {
	char trash;
	if (!file->Read(&trash, sizeof(char))) {
		return false;
	}
	unsigned long long int uncompressed, compressed;
	if (!file->Read((char*)&uncompressed, sizeof(unsigned long long int))) {
		return false;
	}
	if (trash != 'W' && trash != '7' && trash != 'A') {
		return false;
	}
	compressed = file->SizeFile();
	double ratio = (double) compressed / uncompressed;
	ratio = 1 - ratio;
	if (uncompressed == 0) {
		ratio = 0.0;
	}
	else if (ratio < -1) {
		ratio = -1;
	}
	std::cout << "         compressed        uncompressed  ratio uncompressed_name" << std::endl;
	printf("%19llu %19llu %5.1lf", compressed, uncompressed, ratio * 100);
	if (IsArchive(fileName)) {
		fileName.pop_back();
		fileName.pop_back();
		fileName.pop_back();
	}
	std::cout << "% " << fileName << std::endl;
	return true;
}

void WorkWithDirectory(std::string directoryName) {
	DIR *directory = opendir(directoryName.c_str());
	if (errno == ENOTDIR || errno == EACCES || errno == EBADF ||
		errno == EMFILE || errno == ENOMEM || errno == ENOENT)
	{
		if (errno != ENOTDIR) {
			PrintDirectoryErrors(directoryName);
		}
		return;
	}
	struct dirent *directoryFile;
	while (directoryFile = readdir(directory)) {
		if (errno == EBADF) {
			std::cout << directoryName << ": something wrong" << std::endl;
			return;
		}
		std::string tmp = std::string(directoryFile->d_name);
		if (tmp == "." || tmp == "..") {
			continue;
		}
		if (directoryName.back() == '/') {
			tmp = directoryName + tmp;
		}
		else {
			tmp = directoryName + "/" + tmp;
		}
		if (IsDirectory(tmp, true)) {
			WorkWithDirectory(tmp);
		}
		else {
			WorkWithFile(tmp);
		}
	}
	closedir(directory);
	if (errno == EBADF) {
		PrintDirectoryErrors(directoryName);
	}
	return;
}

void WorkWithFile(std::string fileName) {
	InBinary* file = new InBinary;
	if (file == nullptr) {
		std::cout << fileName << ": unexpected memory error" << std::endl;
		return;
	}
	if (!file->Open(&fileName)) {
		std::cout << fileName << ": can't read file" << std::endl;
		delete file;
		return;
	}
	if (keys[3]) {//-l
		if(!DifferensOfSizes(file, fileName)) {
			std::cout << fileName << ": wrong format" << std::endl;
		}
	}
	else if (keys[1] || keys[5]) {//-d или -t
		MainDecompress(file, fileName);
	}
	else {
		MainCompress(file, fileName);
	}
	file->Close();
	delete file;
	return;
}

bool IsDirectory(std::string directoryName, bool help) {
	DIR* directory = opendir(directoryName.c_str());
	if (errno == ENOTDIR || errno == EACCES || errno == EBADF ||
		errno == EMFILE || errno == ENOMEM || errno == ENOENT)
	{
		if (help && errno != ENOTDIR) {
			PrintDirectoryErrors(directoryName);
		}
		return false;
	}
	closedir(directory);
	if (errno == EBADF) {
		if (help) {
			PrintDirectoryErrors(directoryName);
		}
		return false;
	}
	return true;
}

void PrintDirectoryErrors(std::string directoryName) {
	switch (errno) {
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
		case ENOENT:
			std::cout << "No file or directory with name " << directoryName << std::endl;
			break;
	}
	return;
}

bool IsArchive(std::string fileName) {
	int size = fileName.length() - 1;
	return (fileName[size - 2] == '.' && fileName[size - 1] == 'g' && fileName[size] == 'z');
}

void Rename(std::string oldName, std::string nextName) {
	std::string command = "mv " + oldName + " " + nextName;
	system(command.c_str());
	return;
}

void Delete(std::string fileName) {
	std::string command = "rm " + fileName;
	system(command.c_str());
	return;
}

void MainDecompress(InBinary* file, std::string fileName) {
	if (keys[1]) {
		if (!IsArchive(fileName)) {
			std::cout << fileName << ": unknown suffix -- ignored" << std::endl;
			return;
		}
	}
	std::string tmpName = fileName + ".tmp";
	file->Close();
	std::string nextName = fileName;
	nextName.pop_back();
	nextName.pop_back();
	nextName.pop_back();
	if (!keys[0] && !keys[5]) {
		if (file->Open(&nextName)) {
			std::cout << nextName << " is already exists; do you wish to overwrite (y or n)?" << std::endl;
			char choise;
			std::cin >> choise;
			file->Close();
			if (choise != 'Y' && choise != 'y') {
				std::cout << "\tnot overwritten" << std::endl;
				return;
			}
		}
	}
	if (!file->Open(&fileName)) {
		std::cout << fileName << ": can't read file" << std::endl;
		return;
	}
	OutBinary* decompressedFile = nullptr;
	if (!keys[0] && !keys[5]) {
		decompressedFile = new OutBinary;
		if (decompressedFile == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			return;
		}
		if (!decompressedFile->Open(&tmpName)) {
			std::cout << fileName << ": can't transfer data" << std::endl;
			delete decompressedFile;
			return;
		}
	}
	char algorithm = 0;
	bool success;
	if (!file->Read(&algorithm, sizeof(char))) {
		std::cout << fileName << ": can't transfer data" << std::endl;
		if (!keys[0] && !keys[5]) {
			decompressedFile->Close();
			Delete(tmpName);
			delete decompressedFile;
		}
		return;
	}
	if (algorithm == 'A') {//TODO YOU
		/*Arithmetic* method = new Arithmetic();
		if (method == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			if (!keys[0] && !keys[5]) {
				decompressedFile->Close();
				Delete(tmpName);
				delete decompressedFile;
			}
			return;
		}*/
		std::cout << std::endl;//TEST
	}
	else if (algorithm == 'W') {
		TLZW* method = new TLZW(DECOMPRESS, file, decompressedFile);
		if (method == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			if (!keys[0] && !keys[5]) {
				decompressedFile->Close();
				Delete(tmpName);
				delete decompressedFile;
			}
			return;
		}
		success = method->Decompress(fileName);
		delete method;
	}
	else if (algorithm == '7') {//TODO YOU
		/*LZ77* method = new LZ77();
		if (method == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			if (!keys[0] && !keys[5]) {
				decompressedFile->Close();
				Delete(tmpName);
				delete decompressedFile;
			}
			return;
		}
		success = method->Decompress(fileName);
		delete method;*/
		std::cout << std::endl;//TEST
	}
	else {
		std::cout << fileName << ": not compressed data" << std::endl;
		if (!keys[0] && !keys[5]) {
			decompressedFile->Close();
			Delete(tmpName);
			delete decompressedFile;
		}
		return;
	}
	if (!keys[0] && !keys[5]) {
		decompressedFile->Close();
		delete decompressedFile;
	}
	if (!success) {
		if (!keys[5]) {
			std::cout << "\t\tdecompressing failed" << std::endl;
		}
		if (!keys[0] && !keys[5]) {
			Delete(tmpName);
		}
		return;
	}
	if (!keys[0] && !keys[2] && !keys[5]) {//нет -c, -k и -t
		Delete(fileName);
	}
	if (!keys[0] && !keys[5]) {
		Rename(tmpName, nextName);
	}
	return;
}

void MainCompress(InBinary* file, std::string fileName) {
	std::string nextName = fileName + ".gz";
	file->Close();
	if (IsArchive(fileName)) {
		std::cout << fileName << " already has .gz suffix -- unchanged" << std::endl;
		return;
	}
	if (!keys[0]) {
		if (file->Open(&nextName)) {
			std::cout << nextName << " is already exists; do you wish to overwrite (y or n)?" << std::endl;
			char choise;
			std::cin >> choise;
			file->Close();
			if (choise != 'Y' && choise != 'y') {
				std::cout << "\tnot overwritten" << std::endl;
				return;
			}
		}
	}
	int compressRatio = NORMAL;
	if (keys[6]) {
		compressRatio = FAST;
	}
	else if (keys[7]) {
		compressRatio = HIGH;
	}
	OutBinary* compressionFile = nullptr;
	if (!keys[0]) {
		compressionFile = new OutBinary;
		if (compressionFile == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			return;
		}
	}
	unsigned long long int LZWSize, LZ77Size, arithmeticSize;
	/* Блок с моим LZW */
	LZWSize = LZWCompress(file, fileName, compressionFile, compressRatio);
	file->Close();
	if (LZWSize == 0) {
		if (!keys[0]) {
			delete compressionFile;
			Delete(fileName + ".LZW");
		}
		return;
	}
	/* LZ77 */
	/*TODO YOU
	LZ77Size = LZ77Compress(file, fileName, compressionFile);
	file->Close();
	if (LZ77Size == 0) {
		if (!keys[0]) {
			delete compressionFile;
			Delete(fileName + ".LZW");
			Delete(fileName + ".LZ7");
		}
		return;
	}*/
	/* арифметика */
	/* TODO YOU
	arithmeticSize = ArithmeticCompress(file, fileName, compressionFile);
	if (arithmeticSize == 0) {
		if (!keys[0]) {
			Delete(fileName + ".LZW");
			Delete(fileName + ".LZ7");
			Delete(fileName + ".ARI");
		}
		return;
	}*/
	if (!keys[0]) {
		delete compressionFile;
		KeepSmall(LZWSize, LZ77Size, arithmeticSize, fileName);
	}
	return;
}

unsigned long long int LZWCompress(InBinary* file, std::string fileName,
								   OutBinary* compressionFile, int compressRatio) {
	std::string LZWName = fileName + ".LZW";
	if (!file->Open(&fileName)) {
		std::cout << fileName << ": can't read file" << std::endl;
		return 0;
	}
	if (!keys[0]) {
		if (!compressionFile->Open(&LZWName)) {
			std::cout << fileName << ": can't transfer data" << std::endl;
			return 0;
		}
	}
	TLZW* method = new TLZW(compressRatio, file, compressionFile);
	if (method == nullptr) {
		std::cout << fileName << ": unexpected memory error" << std::endl;
		return 0;
	}
	if (!method->Compress(fileName)) {
		delete method;
		std::cout << "\t\tcompression failed" << std::endl;
		return 0;
	}
	delete method;
	compressionFile->Close();
	file->Close();
	if (!file->Open(&LZWName)) {
		std::cout << fileName << ": can't read file" << std::endl;
		return 0;
	}
	return file->SizeFile();
}
/*
unsigned long long int LZ77Compress(InBinary* file, std::string fileName, OutBinary* compressionFile) {
	std::string LZ77Name = fileName + ".LZ7";
	if (!file->Open(&fileName)) {
		std::cout << fileName << ": can't read file" << std::endl;
		return 0;
	}
	if (!keys[0]) {
		if (!compressionFile->Open(&LZ77Name)) {
			std::cout << fileName << ": can't transfer data" << std::endl;
			return 0;
		}
	}
	LZ77* method = new LZ77;
	if (method == nullptr) {
		std::cout << fileName << ": unexpected memory error" << std::endl;
		return 0;
	}
	if (!method->Compress()) {
		delete method;
		std::cout << "\t\tcompression failed" << std::endl;
		return 0;
	}
	delete method;
	compressionFile->Close();
	file->Close();
	if (!file->Open(&LZWName)) {
		std::cout << fileName << ": can't read file" << std::endl;
		return 0;
	}
	return file->SizeFile();
}*/
/*
unsigned long long int ArithmeticCompress(InBinary* file, std::string fileName, OutBinary* compressionFile) {
	std::string arithmeticName	= fileName + ".ARI";
	if (!file->Open(&fileName)) {
		std::cout << fileName << ": can't read file" << std::endl;
		return 0;
	}
	if (!keys[0]) {
		if (!compressionFile->Open(&arithmeticName)) {
			std::cout << fileName << ": can't transfer data" << std::endl;
			return 0;
		}
	}
	Arithmetic* method = new Arithmetic;
	if (method == nullptr) {
		std::cout << fileName << ": unexpected memory error" << std::endl;
		return 0;
	}
	if (!method->Compress()) {
		delete method;
		std::cout << "\t\tcompression failed" << std::endl;
		return 0;
	}
	delete method;
	compressionFile->Close();
	file->Close();
	if (!file->Open(&LZWName)) {
		std::cout << fileName << ": can't read file" << std::endl;
		return 0;
	}
	return file->SizeFile();
}*/

void KeepSmall(unsigned long long int LZWSize, unsigned long long int LZ77Size,
			   unsigned long long int arithmeticSize, std::string fileName) {
	std::string nextName = fileName + ".LZW";//TEST
	/*if (LZWSize < LZ77Size) {//удаление временных файлов YOU
		Delete(LZ77Name);
		if (LZWSize < arithmeticSize) {
			Delete(arithmeticName);
			nextName += ".LZW";
		}
		else {
			Delete(LZWName);
			nextName += ".ARI";
		}
	}
	else {
		Delete(LZWName);
		if (LZ77Size < arithmeticSize) {
			Delete(arithmeticName);
			nextName += ".LZ7";
		}
		else {
			Delete(LZ77Name);
			nextName += ".ARI";
		}
	} *///TODO
	std::string oldName = nextName;
	nextName.pop_back();
	nextName.pop_back();
	nextName.pop_back();
	nextName += "gz";
	Rename(oldName, nextName);
	if (!keys[2]) {//-k
		Delete(fileName);
	}
}
