#include "main_help.h"


/* ключи																				как будут представлены
 * 																				
 * -c вывод архива в out																функция
 * -d распаковка																		bool
 * -k не удалять файлы после компрессии или декомпрессии								bool
 * -l вывод для каждого файла размера сжатого, оригинального,
  	  коэффициента сжатия(%) и имя ориг файла											функция
 * -r рекурсивно разобрать дерево каталогов (сжатие/разжатие всех файлов 
 * 	  в папке но сама папка нетрогается)												функция
 * -t проверить целостность сжатого файла												функция
 * -1 быстро сжать																		bool/int
 * -9 максимум сжатия																	bool/int
 */
 
//	gzip-у пофиг на повторяющиеся ключи						\^_^/

//	но компресить и декомпресить за 1 раз он не могёт
//	TODO Проверка на совпадения и несовпедения имен			/Т_Т\

std::vector<bool> keys;

int main(int argc, char *argv[]) {
	std::vector<std::string> fileNames;
	keys = {false, false, false, false, false, false, false, false};
//			c	   d	  k		 l		r	   t	  1		 9
	for (int i = 1; i < argc; ++i) {
		std::string keyFile;
		std::stringstream tmp(argv[i]);
		tmp >> keyFile;
		if (keyFile[0] == '-') {//ага ключ(и)
			if (!KeyManager(keyFile)) {
				return -1;
			}
		}
		else {// ага файлы и папки
			bool got = false;
			for (int j = 0; j < fileNames.size(); ++j) {
				if (fileNames[j] == keyFile) {//для избежания работы над одним и темже файлом несколько раз
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
		else if (directory) {//TODO есть 2 варианта для этих двух элсов: 1) преобразовать в один-единый 2) внутри работы с директориями юзать работу с файлами 
			WorkWithDirectory(fileNames[i]);
		}
		else {
			WorkWithFile(fileNames[i]);
		}
	}
	for (int i = 0; i < 8; ++i) {//TEST
		std::cout << (keys[i] ? "+" : "-") << std::endl;
	}
	return 0;
}
