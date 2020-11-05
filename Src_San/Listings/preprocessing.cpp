#include "preprocessing.h"

void Compress(std::string fileName)
{
	std::string compressFile = fileName + ".gz";

	if (ArchiveCheck(fileName))
	{
		std::cout << fileName << " already has .gz suffix -- unchanged" << std::endl;
		return;
	}
	
	FILE *file;
	
	if (!keys[0])
		if(file = fopen(compressFile.c_str(), "r"))
		{
			std::cout << compressFile << " is already exists; do you wish to overwrite (y or n)?" << std::endl;
			char save;
			std::cin >> save;
			fclose(file);

			if (save != 'Y' && save != 'y')
			{
				std::cout << "\tnot overwritten" << std::endl;
				return;
			}
		}

	unsigned long long int sizeLZ, sizeAr;

	if (!keys[6] && !keys[7])
	{
		sizeLZ = CompressL(fileName);

		if (sizeLZ == 0)
		{
			if (!keys[0])
				Rm(fileName + ".LZ7");

			return;
		}
		
		sizeAr = CompressA(fileName);

		if (sizeAr == 0)
		{
			if (!keys[0])
			{
				Rm(fileName + ".LZ7");
				Rm(fileName + ".ARI");
			}
			
			return;
		}
		
	}
	else if (keys[6])
	{
		sizeAr = 0;
		sizeLZ = CompressL(fileName);

		if (sizeLZ == 0)
		{
			if (!keys[0])
				Rm(fileName + ".LZ7");

			return;
		}
	}
	else
	{
		sizeLZ = 0;
		sizeAr = CompressA(fileName);

		if (sizeAr == 0)
		{
			if (!keys[0])
				Rm(fileName + ".ARI");

			return;
		}
	}

	if (!keys[0])
		SaveBest(fileName, sizeLZ, sizeAr);
}
void Decompress(std::string fileName)
{
	if (keys[1] && !keys[0])
		if (!ArchiveCheck(fileName))
		{
			std::cout << fileName << ": unknown suffix -- ignored" << std::endl;
			return;
		}

	std::string tmp = fileName + ".tmp";
	std::string decompresFile = fileName;

	decompresFile.pop_back();
	decompresFile.pop_back();
	decompresFile.pop_back();

	FILE* file = NULL;

	if (!keys[0] && !keys[5])
		if (file = fopen(decompresFile.c_str(), "r"))
		{
			std::cout << decompresFile << " is already exists; do you wish to overwrite (y or n)?" << std::endl;
			char save;
			std::cin >> save;
			fclose(file);

			if (save != 'Y' && save != 'y')
			{
				std::cout << "\tnot overwritten" << std::endl;
				return;
			}
		}

	char algo = 0;
	bool complete;

	if (!(file = fopen(fileName.c_str(), "r")))
	{
		std::cout << "Can\'t open file " << fileName << std::endl;
		return;
	}

	if (!fread(&algo, sizeof(char), 1, file))
	{
		std::cout << fileName << ": can't transfer data" << std::endl;
		return;
	}
	fclose(file);
	if (algo == 'A')
	{
		Arithmetic* algorithm = new Arithmetic;

		if (algorithm == nullptr)
		{
			std::cout << fileName << ": unexpected memory error" << std::endl;
			return;
		}

		complete = algorithm->Decompress(fileName.c_str(), tmp.c_str());
		delete algorithm;
	}
	else if (algo == 'L')
	{
		LZ77* algorithm = new LZ77;

		if (algorithm == nullptr)
		{
			std::cout << fileName << ": unexpected memory error" << std::endl;
			return;
		}

		complete = algorithm->Decompress(fileName, tmp);
		delete algorithm;
	}
	else
	{
		std::cout << fileName << ": not compressed data" << std::endl;
		return;
	}

	if (!complete)
	{
		if (!keys[5])
			std::cout << "\t\tdecompressing failed" << std::endl;

		if (!keys[0] && !keys[5])
			Rm(tmp);

		return;
	}

	if (!keys[0] && !keys[2] && !keys[5])
		Rm(fileName);

	if (!keys[0] && !keys[5])
		Mv(tmp, decompresFile);
}
bool ActivateKeys(std::string argvKeys)
{
    for (int j = 1; j < argvKeys.size(); ++j)
    {
        switch (argvKeys[j])
        {
            case 'a':
                keys[8] = true;
                break;
            case 'c':
                if (!keys[3] || !keys[5])
                    keys[0] = true;
                if (keys[0])
                    keys[2] = false;
                break;
            case 'd':
                if (!keys[3] || !keys[5])
                    keys[1] = true;
                if (keys[1])
                {
                    keys[6] = false;
                    keys[7] = false;
                }
                break;
            case 'k':
                if (!keys[0] || !keys[3] || !keys[5])
                    keys[2] = true;
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
                if (!keys[3])
                    keys[5] = true;
                if (keys[5])
                {
                    keys[0] = false;
                    keys[1] = false;
                    keys[2] = false;
                    keys[6] = false;
                    keys[7] = false;
                }
                break;
            case '1':
                if (!keys[1] && !keys[3] && !keys[5])
                    keys[6] = true;
                if (keys[6])
                    keys[7] = false;
                break;
            case '9':
                if (!keys[1] && !keys[3] && !keys[5])
                    keys[7] = true;
                if (keys[7])
                    keys[6] = false;
                break;
            default:
                std::cout << "invalid option -- '" << argvKeys[j] << "'" << std::endl;
                return false;
        }
    }
    return true;
}
void ArchiveInfo(std::string fileName)
{
    char algo = 0;
	FILE *file;
	if(!(file = fopen(fileName.c_str(), "r")))
	{
		std::cout << "Can't open file " << fileName << std::endl;
		return;
	}
    
    if (!fread(&algo, sizeof(char), 1, file))
	{
		std::cout << "Can't read file " << fileName << std::endl;
		return;
	}
	
    if (algo != 'L' && algo != 'A')
	{
		std::cout << fileName << ": wrong format" << std::endl;
		return;
	}

    unsigned long long int big, cut;

    if (!fread(&big, sizeof(unsigned long long int), 1, file))
	{
		std::cout << "Can't read file " << fileName << std::endl;
		return;
	}

	if (fseek(file, 0L, SEEK_END))
	{
		std::cout << "Can't read file " << fileName << std::endl;
		return;
	}
	
    cut = ftell(file);

	fclose(file);
	
    double ratio = (double) cut / big;
    ratio = 1 - ratio;

    if (big == 0)
        ratio = 0.0;
    else if (ratio < -1)
        ratio = -1;

    std::cout << "         compressed        uncompressed  ratio uncompressed_name" << std::endl;

    printf("%19llu %19llu %5.1lf", cut, big, ratio * 100);

    if (ArchiveCheck(fileName))
    {
        fileName.pop_back();
        fileName.pop_back();
        fileName.pop_back();
    }

    std::cout << "% " << fileName << std::endl;
}
unsigned long long int CompressA(std::string fileName)
{
    std::string tmp  = fileName + ".ARI";
    Arithmetic* algorithm = new Arithmetic;

    if (algorithm == nullptr)
    {
        std::cout << fileName << ": unexpected memory error" << std::endl;
        return 0;
    }

    if (!algorithm->Compress(fileName.c_str(), tmp.c_str()))
    {
        delete algorithm;
        std::cout << "\t\tcompression failed" << std::endl;
        return 0;
    }

    delete algorithm;

    if (keys[0])
        return 1;

	FILE *file;
	
	if (!(file = fopen(tmp.c_str(), "r")))
	{
		std::cout << fileName << ": can't read file" << std::endl;
        return 0;
	}

	if (fseek(file, 0L, SEEK_END))
	{
		std::cout << "Can't read file " << fileName << std::endl;
		return 0;
	}
	
    unsigned long long int size = ftell(file);

	fclose(file);

    return size;
}
unsigned long long int CompressL(std::string fileName)
{
    std::string tmp = fileName + ".LZ7";

    LZ77* algorithm = new LZ77;

    if (algorithm == nullptr)
    {
        std::cout << fileName << ": unexpected memory error" << std::endl;
        return 0;
    }

    if (!algorithm->Compress(fileName, tmp))
    {
        delete algorithm;
        std::cout << "\t\tcompression failed" << std::endl;
        return 0;
    }

    delete algorithm;

    if (keys[0])
        return 1;

    FILE *file;
	
	if (!(file = fopen(tmp.c_str(), "r")))
	{
		std::cout << fileName << ": can't read file" << std::endl;
        return 0;
	}

	if (fseek(file, 0L, SEEK_END))
	{
		std::cout << "Can't read file " << fileName << std::endl;
		return 0;
	}
	
    unsigned long long int size = ftell(file);

	fclose(file);

    return size;
}
bool ArchiveCheck(std::string fileName)
{
    int length = fileName.length() - 1;
    return (fileName[length - 2] == '.' && fileName[length - 1] == 'g' && fileName[length] == 'z');
}
bool DirectoryCheck(std::string directoryName, bool help)
{
	DIR* directory = opendir(directoryName.c_str());
	if (directory == NULL)
	{
		if (help && errno != ENOTDIR)
			ShowErrors(directoryName);
		return false;
	}

	closedir(directory);

	if (errno == EBADF)
	{
		if (help)
			ShowErrors(directoryName);
		return false;
	}

	return true;
}
void GetFiles(std::string directoryName, std::map<std::string, int>* filesInDirectories)
{
	DIR *directory = opendir(directoryName.c_str());

	if (directory == NULL)
	{
		ShowErrors(directoryName);
		return;
	}

	short int depth = 0;

	for (int i = 0; i < directoryName.size(); ++i)
	{
		if (directoryName[i] == '/')
			++depth;

		if (depth > 1)
			break;
	}

	struct dirent *directoryFile = readdir(directory);
	std::map<std::string, int>::iterator finder;

	while (directoryFile)
	{
		if (errno == EBADF)
		{
			std::cout << directoryName << ": something wrong" << std::endl;
			return;
		}

		std::string tmp = std::string(directoryFile->d_name);

		if (tmp == "." || tmp == ".." || (tmp == "main" && depth == 1 && directoryName[1] == '.'
			&& (directoryName[0] == directoryName[1])))
		{
			directoryFile = readdir(directory);
			continue;
		}

		if (directoryName.back() == '/')
			tmp = directoryName + tmp;
		else
			tmp = directoryName + "/" + tmp;

		if (tmp == "./main")
		{
			directoryFile = readdir(directory);
			continue;
		}

		if (DirectoryCheck(tmp, true))
			GetFiles(tmp, filesInDirectories);
		else
		{
			bool got = false;
			finder = filesInDirectories->find(tmp);

			if (finder != filesInDirectories->end())
				got = true;

			if (!got)
				filesInDirectories->insert({tmp, filesInDirectories->size() + 1});
		}

		directoryFile = readdir(directory);
	}

	closedir(directory);

	if (errno == EBADF)
		ShowErrors(directoryName);
}
void SaveBest(std::string fileName, unsigned long long int sizeLZ, unsigned long long int sizeAr)
{
    std::string gz;

    if (sizeLZ > 0 && sizeAr > 0)
    {
        if (sizeLZ > sizeAr)
        {
            gz = fileName + ".ARI";
            Rm(fileName + ".LZ7");
        }
        else
        {
            gz = fileName + ".LZ7";
            Rm(fileName + ".ARI");
        }

        Mv(gz, fileName + ".gz");
    }
    else
        if (sizeLZ == 0)
            Mv(fileName + ".ARI", fileName + ".gz");
        else 
            Mv(fileName + ".LZ7", fileName + ".gz");

    if (!keys[2])
        Rm(fileName);
}
void ShowErrors(std::string directoryName)
{
    switch (errno)
    {
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
}
void Mv(std::string oldName, std::string newName)
{
    std::string command = "mv ./" + oldName + " ./" + newName;
    system(command.c_str());
}
void Rm(std::string fileName)
{
    std::string command = "rm ./" + fileName;
    system(command.c_str());
}
