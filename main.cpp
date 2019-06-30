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

int main(int argc, char *argv[]) {
	std::vector<std::string> fileNames;
	std::vector<bool> keys = {false, false, false, false, false, false, false, false};
	std::vector<char> usedKeys = {'c', 'd', 'k', 'l', 'r', 't', '1', '9'};
	for (int i = 1; i < argc; ++i) {
		std::string keyFile, kkk;
		std::stringstream tmp(argv[i]);
		tmp >> keyFile;
		std::stringstream secondTmp(keyFile);
		char file[keyFile.size() + 1];
		secondTmp >> file;
		if (keyFile[0] == '-') {//ага ключ(и)
			for (int j = 1; j < keyFile.size(); ++j) {
				switch (file[j]) {//TODO блядский свич не пашет с чарами. оставить '' или продолжить хавать кактус?
					case /*usedKeys[0]*/'c': {
						if (!keys[3] || !keys[5]) {
							keys[0] = true;
						}
						if (keys[0]) {
							keys[2] = false;
						}
						break;
					}
					case /*usedKeys[1]*/'d': {
						if (!keys[3] || !keys[5]) {
							keys[1] = true;
						}
						if (keys[1]) {
							keys[6] = false;
							keys[7] = false;
						}
						break;
					}
					case /*usedKeys[2]*/'k': {
						if (!keys[0] || !keys[3] || !keys[5]) {
							keys[2] = true;
						}
						break;
					}
					case /*usedKeys[3]*/'l': {
						keys[3] = true;
						keys[0] = false;
						keys[1] = false;
						keys[2] = false;
						keys[5] = false;
						keys[6] = false;
						keys[7] = false;
						break;
					}
					case /*usedKeys[4]*/'r': {
						keys[4] = true;
						break;
					}
					case /*usedKeys[5]*/'t': {
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
					case /*usedKeys[6]*/'1': {
						if (!keys[1] && !keys[3] && !keys[5]) {
							keys[6] = true;
						}
						if (keys[6]) {
							keys[7] = false;
						}
						break;
					}
					case /*usedKeys[7]*/'9': {
						if (!keys[1] && !keys[3] && !keys[5]) {
							keys[7] = true;
						}
						if (keys[7]) {
							keys[6] = false;
						}
						break;
					}
					default: {
						std::cout << "No such key '" << keyFile[j] << "'" << std::endl << "Fuck you" << std::endl;
						return -1;
					}
				}
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
	/* TODO надо ли здесь упростить usedKeys до простого вектора булов? или так и оставим?
	 * TODO мож их вообще в глобалы отправить?*/
	for (int i = 0; i < fileNames.size(); i++) {//проверка на файлы и папки и дальнейшая работа с ними
		bool directory = IsDirectory(fileNames[i]);
		if (!keys[4] && directory) {
			std::cout << fileNames[i] << " is a directory -- ignored" << std::endl;
		}
		else if (directory) {//TODO есть 2 варианта для этих двух элсов: 1) преобразовать в один-единый 2) внутри работы с директориями юзать работу с файлами 
			WorkWithDirectory(fileNames[i], keys);
		}
		else {
			WorkWithFile(fileNames[i], keys);
		}
	}
	return 0;
}
