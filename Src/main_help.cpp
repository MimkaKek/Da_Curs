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

void ShowResult(std::string fileName) {
	InBinary* file = new InBinary;
	if (file == nullptr) {
		std::cout << fileName << ": unexpected memory error" << std::endl;
		return;
	}
	if (!file->Open(&fileName)) {
		std::cout << fileName << "can't read file" << std::endl;
		delete file;
		return;
	}
	char outPut;
	while (file->Read(&outPut, CHAR)) {
		std::cout << outPut;
	}
	std::cout << std::endl;
	file->Close();
	delete file;
	return;
}

bool DifferensOfSizes(InBinary* file, std::string fileName) {
	char trash;
	if (!file->Read(&trash, CHAR)) {
		return false;
	}
	unsigned long long int uncompressed, compressed;
	if (!file->Read((char*)&uncompressed, LLINT)) {
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
	fileName.pop_back();
	fileName.pop_back();
	fileName.pop_back();
	std::cout << "% " << fileName << std::endl;
	return true;
}

void WorkWithDirectory(std::string directoryName) {//TODO
	DIR* directory = opendir(directoryName.c_str());
	if (errno == ENOTDIR || errno == EACCES || errno == EBADF ||
		errno == EMFILE || errno == ENOMEM || errno == ENOENT)
	{
		if (errno != ENOTDIR) {
			PrintDirectoryErrors(directoryName);
		}
		return;
	}
	struct dirent* directoryFile = readdir(directory);
	while (directoryFile != NULL) {
		std::string tmp = directoryName + std::string(directoryFile->d_name);
		std::cout << tmp << std::endl;//TEST
		if (IsDirectory(tmp, true)) {
			WorkWithDirectory(tmp);
		}
		else {
			WorkWithFile(tmp);
		}
		directoryFile = readdir(directory);
	}
	closedir(directory);
	if (errno == EBADF) {
		PrintDirectoryErrors(directoryName);
	}
	return;
}

void WorkWithFile(std::string fileName) {
	std::string nextName;
	InBinary* file = new InBinary;
	if (file == nullptr) {
		std::cout << fileName << ": unexpected memory error" << std::endl;
		return;
	}
	if (!file->Open(&fileName)) {
		std::cout << fileName << ":\tNo such file" << std::endl;
		delete file;
		return;
	}
	if (keys[3]) {//-l (L)
		if (!IsArchive(fileName)) {
			std::cout << fileName << " is not an archive" << std::endl;
		}
		else {
			if(!DifferensOfSizes(file, fileName)) {
				std::cout << fileName << ": can't read file" << std::endl;
			}
		}
		file->Close();
		delete file;
	}
	else if (keys[1] || keys[5]) {//-d или -t
		std::string tmpName = fileName + ".tmp";
		file->Close();
		nextName = fileName;
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
					delete file;
					return;
				}
			}
		}
		if (!file->Open(&fileName)) {
			std::cout << fileName << ": can't read file" << std::endl;
			delete file;
			return;
		}
		OutBinary* decompressedFile = new OutBinary;
		if (decompressedFile == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			file->Close();
			delete file;
			return;
		}
		if (!keys[0] && !keys[5]) {
			if (!decompressedFile->Open(&tmpName)) {
				std::cout << fileName << ": can't transfer data" << std::endl;
				file->Close();
				delete decompressedFile;
				delete file;
				return;
			}
		}
		char algorithm;
		bool success;
		if (!file->Read(&algorithm, CHAR)) {
			std::cout << fileName << ": can't transfer data" << std::endl;
			file->Close();
			if (!keys[0] && !keys[5]) {
				decompressedFile->Close();
				Delete(tmpName);
			}
			delete decompressedFile;
			delete file;
			return;
		}
		switch (algorithm) {//TODO switch
			case 'A': {
				//Arithmetic* method = new Arithmetic(); TODO
				std::cout << std::endl;//TEST
				break;
			}
			case 'W': {
				TLZW* method = new TLZW(DECOMPRESS, file, decompressedFile);
				if (method == nullptr) {
					std::cout << fileName << ": unexpected memory error" << std::endl;
					file->Close();
					if (!keys[0] && !keys[5]) {
						decompressedFile->Close();
						Delete(tmpName);
					}
					delete decompressedFile;
					delete file;
					return;
				}
				success = method->Decompress(fileName);
				delete method;
				break;
			}
			case '7': {
				//LZ77* method = new LZ77(); TODO
				std::cout << std::endl;//TEST
				break;
			}
			default: {
				std::cout << fileName << ": not compressed data" << std::endl;
				file->Close();
				if (!keys[0] && !keys[5]) {
					decompressedFile->Close();
					Delete(tmpName);
				}
				delete decompressedFile;
				delete file;
				return;
			}
		}
		file->Close();
		if (!keys[0] && !keys[5]) {
			decompressedFile->Close();
		}
		delete decompressedFile;
		delete file;
		if (!success) {
			std::cout << "\t\tdecompressing failed" << std::endl;
			if (!keys[0] && !keys[5]) {
				Delete(tmpName);
			}
			return;
		}
		if (!keys[0] && !keys[2] && !keys[5]) {//-c или -k или -t
			Delete(fileName);
		}
		if (!keys[0] && !keys[5]) {
			Rename(tmpName, nextName);
		}
	}
	else {
		file->Close();
		if (IsArchive(fileName)) {
			std::cout << fileName << " already has .gz suffix -- unchanged" << std::endl;
			return;
		}
		nextName = fileName + ".gz";
		if (!keys[0]) {
			if (file->Open(&nextName)) {
				std::cout << nextName << " is already exists; do you wish to overwrite (y or n)?" << std::endl;
				char choise;
				std::cin >> choise;
				file->Close();
				if (choise != 'Y' && choise != 'y') {
					std::cout << "\tnot overwritten" << std::endl;
					delete file;
					return;
				}
			}
		}
		if (!file->Open(&fileName)) {
			std::cout << fileName << ": can't read file" << std::endl;
			delete file;
			return;
		}
		int compressRatio = NORMAL;
		if (keys[6]) {
			compressRatio = FAST;
		}
		else if (keys[7]) {
			compressRatio = HIGH;
		}
		OutBinary* compressionFile = new OutBinary;
		if (compressionFile == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			file->Close();
			delete file;
		}
		std::string LZWName			= fileName + ".LZW";
		std::string LZ77Name		= fileName + ".LZ7";
		std::string arithmeticName	= fileName + ".ARI";
		unsigned long long int LZWSize, LZ77Size, arithmeticSize;
		/* Блок с моим LZW */
		if (!keys[0]) {
			if (!compressionFile->Open(&LZWName)) {
				file->Close();
				delete compressionFile;
				delete file;
				std::cout << fileName << ": can't transfer data" << std::endl;
				return;
			}
		}
		TLZW* LZW = new TLZW(compressRatio, file, compressionFile);
		if (LZW == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			file->Close();
			compressionFile->Close();
			Delete(LZWName);
			delete compressionFile;
			delete file;
			return;
		}
		if (!LZW->Compress(fileName)) {
			file->Close();
			compressionFile->Close();
			Delete(LZWName);
			delete LZW;
			delete compressionFile;
			delete file;
			std::cout << "\t\tcompression failed";
			return;
		}
		delete LZW;
		LZWSize = compressionFile->SizeFile();
		compressionFile->Close();
		file->Close();
		/* Блок с вашими алгоритмами */ //TODO
		/*							 */
		
		nextName = LZWName;//TEST
		/*if (LZWSize < LZ77Size) {//удаление временных файлов
			Delete(LZ77Name);
			if (LZWSize < arithmeticSize) {
				Delete(arithmeticName);
				nextName = LZWName;
			}
			else {
				Delete(LZWName);
				nextName = arithmeticName;
			}
		}
		else {
			Delete(LZWName);
			if (LZ77Size < arithmeticSize) {
				Delete(arithmeticName);
				nextName = LZ77Name;
			}
			else {
				Delete(LZ77Name);
				nextName = arithmeticName;
			}
		}*/ //TODO
		std::string oldName = nextName;
		nextName.pop_back();
		nextName.pop_back();
		nextName.pop_back();
		nextName += "gz";
		if (!keys[0]) {
			Rename(oldName, nextName);
			if (!keys[2]) {//-k
				Delete(fileName);
			}
		}
	}
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
		PrintDirectoryErrors(directoryName);
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
