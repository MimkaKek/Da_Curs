/* main_help.cpp */
#include "main_help.h"//TODO W -> L

bool KeyManager(std::string gotKeys) {
    for (int j = 1; j < gotKeys.size(); ++j) {
        switch (gotKeys[j]) {
            case 'a':
                keys[8] = true;
                break;
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

bool DifferensOfSizes(TInBinary* file, std::string fileName) {
    char trash = 0;
    if (!file->Read(&trash, sizeof(char))) {
        return false;
    }
    if (trash != 'L' && trash != 'A') {
        return false;
    }
    unsigned long long int uncompressed, compressed;
    if (!file->Read((char*)&uncompressed, sizeof(unsigned long long int))) {
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

void WorkWithDirectory(std::string directoryName, std::map<std::string, int>* directoryNames) {//TODO
	DIR *directory = opendir(directoryName.c_str());
	if (directory == NULL) {
		PrintDirectoryErrors(directoryName);
		return;
	}
	short int countOfDirs = 0;
	for (int i = 0; i < directoryName.size(); ++i) {
		if (directoryName[i] == '/') {
			++countOfDirs;
		}
		if (countOfDirs > 1) {
			break;
		}
	}
	struct dirent *directoryFile;
	directoryFile = readdir(directory);
	std::map<std::string, int>::iterator finder;
	while (directoryFile) {
		if (errno == EBADF) {
			std::cout << directoryName << ": something wrong" << std::endl;
			return;
		}
		std::string tmp = std::string(directoryFile->d_name);
		if (tmp == "." || tmp == ".." || (tmp == "main" && countOfDirs == 1
			&& (directoryName[0] == directoryName[1]) && directoryName[1] == '.'))
		{
			directoryFile = readdir(directory);
			continue;
		}
		if (directoryName.back() == '/') {
			tmp = directoryName + tmp;
		}
		else {
			tmp = directoryName + "/" + tmp;
		}
		if (tmp == "./main") {
			directoryFile = readdir(directory);
			continue;
		}
		if (IsDirectory(tmp, true)) {
			WorkWithDirectory(tmp, directoryNames);
		}
		else {
			bool got = false;
			finder = directoryNames->find(tmp);
			if (finder != directoryNames->end()) {//для избежания работы над одним и тем же файлом несколько раз
				got = true;
			}
			if (!got) {
				directoryNames->insert({tmp, directoryNames->size + 1});
			}
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
    TInBinary* file = new TInBinary;
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
	if (directory == NULL) {
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
            std::cout << "No permission for directory " << directoryName
                      << "\t Try it next time" << std::endl;
            break;
        case EBADF:
            std::cout << "Not a valid descriptor for directory " << directoryName
                      << "\t Try it next time" << std::endl;
            break;
        case EMFILE:
            std::cout << "Too many files opened in system. Can't open directory" << directoryName
                      << "\t Try it next time" << std::endl;
            break;
        case ENOMEM:
            std::cout << "Not enought memory for opening directory " << directoryName
                      << "\t Try it next time" << std::endl;
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
    std::string command = "mv ./" + oldName + " ./" + nextName;
    system(command.c_str());
    return;
}

void Delete(std::string fileName) {
    std::string command = "rm ./" + fileName;
    system(command.c_str());
    return;
}

void MainDecompress(TInBinary* file, std::string fileName) {
	if (keys[1] && !keys[0]) {
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
	TOutBinary* decompressedFile = nullptr;
	if (!keys[0] && !keys[5]) {
		decompressedFile = new TOutBinary;
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
	if (algorithm == 'A') {
		Arithmetic* method = new Arithmetic;
		if (method == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			if (!keys[0] && !keys[5]) {
				decompressedFile->Close();
				Delete(tmpName);
				delete decompressedFile;
			}
			return;
		}
		success = method->Decompress(fileName.c_str(), tmpName.c_str());
		delete method;
	}
	else if (algorithm == 'L') {
		LZ77* method = new LZ77;//TODO YOU
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
	if (keys[5]) {
		Delete(tmpName);
		return;
	}
	if (!keys[0] && !keys[2]) {//нет -c, -k и -t
		Delete(fileName);
	}
	if (!keys[0]) {
		Rename(tmpName, nextName);
	}
	return;
}

void MainCompress(TInBinary* file, std::string fileName) {//TODO
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
	TOutBinary* compressionFile = nullptr;
	if (!keys[0]) {
		compressionFile = new TOutBinary;
		if (compressionFile == nullptr) {
			std::cout << fileName << ": unexpected memory error" << std::endl;
			return;
		}
	}
	unsigned long long int LZ77Size, arithmeticSize;
	if (!keys[6] && !keys[7]) {//ключи 1 и 9 не указаны
		/* LZ77 */
		LZ77Size = LZ77Compress(file, fileName, compressionFile);//TODO YOU
		file->Close();
		if (LZ77Size == 0) {
			if (!keys[0]) {
				delete compressionFile;
				Delete(fileName + ".LZ7");
			}
			return;
		}
		/* арифметика */
		arithmeticSize = ArithmeticCompress(fileName, file);
		if (arithmeticSize == 0) {
			if (!keys[0]) {
				delete compressionFile;
				Delete(fileName + ".LZ7");
				Delete(fileName + ".ARI");
			}
			return;
		}
		
	}
	else if (keys[6]) {/* LZ77 т.к. 1*/
		arithmeticSize = 0;
		LZ77Size = LZ77Compress(file, fileName, compressionFile);//TODO
		file->Close();
		if (LZ77Size == 0) {
			if (!keys[0]) {
				delete compressionFile;
				Delete(fileName + ".LZ7");
			}
			return;
		}
	}
	else {// Arif т.к. 9
		LZ77Size = 0;
		arithmeticSize = ArithmeticCompress(fileName, file);
		if (arithmeticSize == 0) {
			if (!keys[0]) {
				delete compressionFile;
				Delete(fileName + ".ARI");
			}
			return;
		}
	}
	if (!keys[0]) {
		delete compressionFile;
		KeepSmall(LZ77Size, arithmeticSize, fileName);
	}
	return;
}

unsigned long long int LZ77Compress(TInBinary* file, std::string fileName, TOutBinary* compressionFile) {//TODO
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
    TLZ77* method = new TLZ77;
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
    if (keys[0]) {
        return 1;
    }
    file->Close();
    if (!file->Open(&LZWName)) {
        std::cout << fileName << ": can't read file" << std::endl;
        return 0;
    }
    return file->SizeFile();
}

unsigned long long int ArithmeticCompress(std::string fileName, TInBinary* file) {
    std::string arithmeticName  = fileName + ".ARI";
    Arithmetic* method = new Arithmetic;
    if (method == nullptr) {
        std::cout << fileName << ": unexpected memory error" << std::endl;
        return 0;
    }
    if (!method->Compress(fileName.c_str(), arithmeticName.c_str())) {
        delete method;
        std::cout << "\t\tcompression failed" << std::endl;
        return 0;
    }
    delete method;
    if (keys[0]) {
        return 1;
    }
    if (!file->Open(&arithmeticName)) {
        std::cout << fileName << ": can't read file" << std::endl;
        return 0;
    }
    return file->SizeFile();
}

void KeepSmall(unsigned long long int LZ77Size, unsigned long long int arithmeticSize, std::string fileName) {
    std::string nextName;
    if (LZ77Size > 0 && arithmeticSize > 0) {
        if (LZ77Size > arithmeticSize) {
            nextName = fileName + ".ARI";
            Delete(fileName + ".LZ7");
        }
        else {
            nextName = fileName + ".LZ7";
            Delete(fileName + ".ARI");
        }
        Rename(nextName, fileName + ".gz");
    }
    else {
        if (LZ77Size == 0) {
            Rename(fileName + ".ARI", fileName + ".gz");
        }
        else {
            Rename(fileName + ".LZ7", fileName + ".gz");
        }
    }
    if (!keys[2]) {//-k
        Delete(fileName);
    }
    return;
}
