#include "interface.h"

void FileIterator(std::map<std::string, int> files) {
	std::map<std::string, int>::iterator i;
	bool directory;
	for (i = files.begin(); i != files.end(); ++i) {
		directory = AskDir(i->first, false);
		if (!keys[4] && directory) {
			std::cout << i->first << " is a directory -- ignored\n";
		}
		else if (directory) {
			DirectoryWork(i->first);
		}
		else if (i->first == "main") {
			continue;
		}
		else if (errno == ENOTDIR) {
			DeComPress(i->first);
		}
		else {
			ErrorNotes(i->first);
		}
	}
	return;
}

bool Parser(std::map<std::string, int>* fileNames, std::string argv) {
	if (argv[0] != '-') {
		if (argv[0] == '.' && argv[1] == '/' && argv.size() > 2) {
			argv.erase(0, 2);
		}
		if (fileNames->find(argv) == fileNames->end()) {
			fileNames->insert({argv, fileNames->size() + 1});
		}
		return true;
	}
    for (int j = 1; j < argv.size(); ++j) {
        switch (argv[j]) {
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
                std::cout << "invalid option -- '" << argv[j] << "'\n";
                return false;
        }
    }
    return true;
}

bool AskDir(std::string directoryName, bool help) {
	DIR* directory = opendir(directoryName.c_str());
	if (directory == NULL) {
		if (help && errno != ENOTDIR) {
			ErrorNotes(directoryName);
		}
		return false;
	}
	closedir(directory);
	if (errno == EBADF) {
		if (help) {
			ErrorNotes(directoryName);
		}
		return false;
	}
	return true;
}

void DirectoryWork(std::string dirName) {
	short int directoriesIn = 0;
	for (int i = 0; i < dirName.size(); ++i) {
		if (dirName[i] == '/') {
			++directoriesIn;
		}
		if (directoriesIn > 1) {
			break;
		}
	}
	DIR *directory = opendir(dirName.c_str());
	if (directory == NULL) {
		ErrorNotes(dirName);
		return;
	}
	struct dirent *file = readdir(directory);
	bool programm;
	while (file) {
		if (errno == EBADF) {
			std::cout << dirName << ": something wrong\n";
			return;
		}
		std::string fileName = std::string(file->d_name);
		programm = fileName == "main" && directoriesIn == 1 && dirName[0] == dirName[1] && dirName[1] == '.';
		if (fileName == "." || fileName == ".." || programm) {
			file = readdir(directory);
			continue;
		}
		if (dirName.back() == '/') {
			fileName = dirName + fileName;
		}
		else {
			fileName = dirName + "/" + fileName;
		}
		if (fileName == "./main") {
			file = readdir(directory);
			continue;
		}
		if (AskDir(fileName, true)) {
			DirectoryWork(fileName);
		}
		else {
			DeComPress(fileName);
		}
		file = readdir(directory);
	}
	closedir(directory);
	if (errno == EBADF) {
		ErrorNotes(dirName);
	}
	return;
}

void DeComPress(std::string fileName) {
    TInBinary* file = new TInBinary;
    if (file == nullptr) {
        std::cout << fileName << ": unexpected memory error\n";
        exit(1);
    }
    if (!file->Open(&fileName)) {
        std::cout << fileName << ": can't read file\n";
        delete file;
        return;
    }
    if (keys[3]) {//-l
        if(!KeyL(file, fileName)) {
            std::cout << fileName << ": wrong format\n";
        }
    }
    else if (keys[1] || keys[5]) {//-d или -t
        PreDecompress(file, fileName);
    }
    else {
        PreCompress(file, fileName);
    }
    file->Close();
    delete file;
    return;
}

bool Rewrite(std::string file) {
	std::cout << file << " is already exists; do you wish to overwrite (y or n)?\n";
	char input;
	std::cin >> input;
	if (input != 'Y' && input != 'y') {
		std::cout << "\tnot overwritten\n";
		return false;
	}
	return true;
}

void ErrorNotes(std::string dir) {
	std::string tmp = " directory " + dir + "\t Try it next time\n";
    switch (errno) {
        case EACCES:
            std::cout << "No permission for" << tmp;
            break;
        case EBADF:
            std::cout << "Not a valid descriptor for" << tmp;
            break;
        case EMFILE:
            std::cout << "Too many files opened in system. Can't open" << tmp;
            break;
        case ENOMEM:
            std::cout << "Not enought memory for opening" << tmp;
            break;
        case ENOENT:
            std::cout << "No file or directory with name " << dir << "\n";
            break;
    }
    return;
}

bool KeyL(TInBinary* archive, std::string archiveName) {
    char letter = 0;
    if (!archive->Read(&letter, sizeof(char))) {
        return false;
    }
    if (letter != 'L' && letter != 'A') {
        return false;
    }
    unsigned long long int original, compressed;
    if (!archive->Read((char*)&original, sizeof(unsigned long long int))) {
        return false;
    }
    compressed = archive->SizeFile();
    double coef = 1.0 - (double) compressed / original;
    if (original == 0) {
        coef = 0.0;
    }
    else if (coef < -1) {
        coef = -1.0;
    }
    std::cout << "         compressed        uncompressed  ratio uncompressed_name\n";
    printf("%19llu %19llu %5.1lf", compressed, original, coef * 100);
    if (archiveName.substr(archiveName.length() - 3) == ".gz") {
		archiveName.erase(archiveName.size() - 3, 3);//CHECK
    }
    std::cout << "% " << archiveName << "\n";
    return true;
}

void PreCompress(TInBinary* file, std::string fileName) {//TODO
	std::string archiveName = fileName + ".gz";
	file->Close();
	if (fileName.substr(fileName.length() - 3) == ".gz") {
		std::cout << fileName << " already has .gz suffix -- unchanged\n";
		return;
	}
	if (!keys[0] && file->Open(&archiveName)) {
		file->Close();
		if (!Rewrite(archiveName)) {
			return;
		}
	}
	file->Close();//CHECK
	unsigned long long int LZ77, arithmetic;
	if (!keys[6] && !keys[7]) {//ключи 1 и 9 не указаны
		/* арифметика */
		arithmetic = Compress(fileName, file, false);
		if (arithmetic == 0) {
			if (!keys[0]) {
				archiveName = "rm ./" + fileName + ".A";
				system(archiveName.c_str());
			}
			return;
		}
		/* LZ77 */
		LZ77 = Compress(fileName, file, true);//TODO YOU
		file->Close();
		if (LZ77 == 0) {
			if (!keys[0]) {
				archiveName = "rm ./" + fileName + ".L " + fileName + ".A";
				system(archiveName.c_str());
			}
			return;
		}
	}
	else if (keys[6]) {/* LZ77 т.к. 1*/
		arithmetic = 0;
		LZ77 = Compress(fileName, file, true);//TODO
		file->Close();
		if (LZ77 == 0) {
			if (!keys[0]) {
				archiveName = "rm ./" + fileName + ".L";
				system(archiveName.c_str());
			}
			return;
		}
	}
	else {// Arif т.к. 9
		LZ77 = 0;
		arithmetic = Compress(fileName, file, false);
		if (arithmetic == 0) {
			if (!keys[0]) {
				archiveName = "rm ./" + fileName + ".A";
				system(archiveName.c_str());
			}
			return;
		}
	}
	if (keys[0]) {
		return;
	}
	archiveName = "";
	if (!keys[2]) {//-k
		archiveName += "rm ./" + fileName + "\n";
    }
    if (LZ77 == 0) {
		archiveName += "mv ./" + fileName + ".A ./" + fileName + ".gz";
    }
    else if (arithmetic == 0) {
		archiveName += "mv ./" + fileName + ".L ./" + fileName + ".gz";
	}
	else if (LZ77 > arithmetic) {
		archiveName += "mv ./" + fileName + ".L ./" + fileName + ".gz\nrm ./" + fileName + ".A";
	}
	else {
		archiveName += "mv ./" + fileName + ".A ./" + fileName + ".gz\nrm ./" + fileName + ".A";
	}
	system(archiveName.c_str());
	return;
}

unsigned long long int Compress(std::string fileName, TInBinary* file, bool LZ) {
    std::string tmpName;
    if (LZ) {
		/*tmpName = fileName + ".L";
		TLZ77* algorithm = new TLZ77;
		if (algorithm == nullptr) {
			std::cout << fileName << ": unexpected memory error\n";
			exit(1);
		}
		if (!algorithm->Compress(fileName.c_str(), tmpName.c_str())) {
			delete algorithm;
			std::cout << "\t\tcompression failed\n";
			return 0;
		}
		delete algorithm;*/
	}
	else {
		tmpName = fileName + ".A";
		ACC* algorithm = new ACC;
		if (algorithm == nullptr) {
			std::cout << fileName << ": unexpected memory error\n";
			exit(1);
		}
		if (!algorithm->Compress(fileName.c_str(), tmpName.c_str())) {
			delete algorithm;
			std::cout << "\t\tcompression failed\n";
			return 0;
		}
		delete algorithm;
	}
	if (keys[0]) {
		return 1;
	}
    if (!file->Open(&tmpName)) {
        std::cout << fileName << ": can't read file\n";
        return 0;
    }
    return file->SizeFile();
}

void PreDecompress(TInBinary* archive, std::string archiveName) {//-t
	if (keys[1] && !keys[0]) {
		if (!(archiveName.substr(archiveName.length() - 3) == ".gz")) {
			std::cout << archiveName << ": unknown suffix -- ignored\n";
			return;
		}
	}
	std::string decompressName = archiveName + ".t";
	archive->Close();
	std::string fileName = archiveName;
	fileName.erase(fileName.size() - 3, 3);//CHECK
	if (!keys[0] && !keys[5] && archive->Open(&fileName)) {
		archive->Close();
		if (!Rewrite(fileName)) {
			return;
		}
	}
	archive->Close();//CHECK
	if (!archive->Open(&archiveName)) {
		std::cout << archiveName << ": can't read file\n";
		return;
	}
	char code = 0;
	bool decompression;
	if (!archive->Read(&code, sizeof(char))) {
		std::cout << archiveName << ": can't transfer data\n";
		return;
	}
	if (code == 'A') {
		ACC* algorithm = new ACC;
		if (algorithm == nullptr) {
			std::cout << archiveName << ": unexpected memory error\n";
			exit(1);
		}
		decompression = algorithm->Decompress(archiveName.c_str(), decompressName.c_str());
		delete algorithm;
	}
	/*else if (code == 'L') {
		TLZ77* algorithm = new TLZ77;//TODO YOU
		if (algorithm == nullptr) {
			std::cout << archiveName << ": unexpected memory error\n";
			exit(1);
		}
		//decompression = algorithm->Decompress(archiveName);
		delete algorithm;
	}*/
	else {
		std::cout << archiveName << ": not compressed data\n";
		return;
	}
	if (!decompression) {
		if (!keys[5]) {
			std::cout << "\t\tdecompressing failed\n";
		}
		if (!keys[0] && !keys[5]) {
			decompressName = "rm ./" + decompressName;
			system(decompressName.c_str());
		}
		return;
	}
	if (!keys[0] && !keys[2] && !keys[5]) {//нет -c, -k и -t
		decompressName = "rm ./" + archiveName;
	}
	if (!keys[2]) {
		archiveName = "rm ./" + archiveName;
		system(archiveName.c_str());
	}
	if (!keys[0] && !keys[5]) {
		decompressName = "mv ./" + decompressName + " ./" + fileName;
		system(decompressName.c_str());
	}
	return;
}
