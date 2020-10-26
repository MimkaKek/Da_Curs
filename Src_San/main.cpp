/* maip.cpp */
#include "preprocessing.h"

std::vector<bool> keys;

int main(int argc, char *argv[])
{
	clock_t t0 = clock();
	std::map<std::string, int> files, filesInDirectories;
	std::map<std::string, int>::iterator finder;
	keys = {false, false, false, false, false, false, false, false, false};
//			c	   d	  k		 l		r	   t	  1		 9		a

	for (int i = 1; i < argc; ++i)
	{
		std::string keyFile;
		std::stringstream tmp(argv[i]);
		
		tmp >> keyFile;
		
		if (keyFile[0] == '-') //ключ(и)
			if (!ActivateKeys(keyFile)) 
				return -1;
		else
		{// файлы и папки
			if (keyFile.size() > 2) 
				if (keyFile[0] == '.' && keyFile[1] == '/') 
					keyFile.erase(0, 2);
			
			if (DirectoryCheck(keyFile, false))
				GetFiles(keyFile, &filesInDirectories);
			else
			{
				bool got = false;
				finder = files.find(keyFile);
				
				if (finder != files.end()) //для избежания работы над одним и тем же файлом несколько раз
					got = true;
				
				if (!got)
					files.insert({keyFile, i});
			}
		}
	}
	
	if (files.empty() && filesInDirectories.empty())
	{
		std::cout << "Compressed data not written to a terminal." << std::endl;
		return 0;
	}
	
	for (finder = filesInDirectories.begin(); finder != filesInDirectories.end() && keys[4]; ++finder) 
		if (finder->first == "main") 
			continue;
		else 
			if (keys[3]) //-l
				ArchiveInfo(finder->first);
			else if (keys[1] || keys[5]) //-d или -t
				Decompress(finder->first);
			else
				Compress(finder->first);

	for (finder = files.begin(); finder != files.end(); ++finder)
		if (finder->first == "main") 
			continue;
		else 
			if (keys[3]) //-l
				ArchiveInfo(finder->first);
			else if (keys[1] || keys[5]) //-d или -t
				Decompress(finder->first);
			else
				Compress(finder->first);

	clock_t t1 = clock();

	if (keys[8])
		std::cout << std::endl << ((double) t1 - t0) / CLOCKS_PER_SEC << " секунд" << std::endl;

	return 0;
}
