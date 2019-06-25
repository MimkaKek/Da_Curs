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
	std::vector<std::string> filenames;
	std::vector<std::pair<char,bool>> usedKeys = {{'c', false}, {'d', false}, {'k', false}, {'l', false}, 
												  {'r', false}, {'t', false}, {'1', false}, {'9', false}};
	for (int i = 1; i < argc; ++i) {
		std::string keyFile;
		std::stringstream tmp(argv[i]);
		tmp >> keyFile;
		if (keyFile[0] == '-') {//ага ключ(и)
			for (int j = 1; j < keyFile.size(); ++j) {//TODO какая-то херня с векторами в кейсах
				switch (keyFile[j]) {
					case /*usedKeys[0].first*/'c': {
						if (!usedKeys[3].second || !usedKeys[5].second) {
							usedKeys[0].second = true;
						}
						if (usedKeys[0].second) {
							usedKeys[2].second = false;
						}
						break;
					}
					case /*usedKeys[1].first*/'d': {
						if (!usedKeys[3].second || !usedKeys[5].second) {
							usedKeys[1].second = true;
						}
						if (usedKeys[1].second) {
							usedKeys[6].second = false;
							usedKeys[7].second = false;
						}
						break;
					}
					case /*usedKeys[2].first*/'k': {
						if (!usedKeys[0].second || !usedKeys[3].second || !usedKeys[5].second) {
							usedKeys[2].second = true;
						}
						break;
					}
					case /*usedKeys[3].first*/'l': {
						usedKeys[3].second = true;
						usedKeys[0].second = false;
						usedKeys[1].second = false;
						usedKeys[2].second = false;
						usedKeys[5].second = false;
						usedKeys[6].second = false;
						usedKeys[7].second = false;
						break;
					}
					case /*usedKeys[4].first*/'r': {
						usedKeys[4].second = true;
						break;
					}
					case /*usedKeys[5].first*/'t': {
						if (!usedKeys[3].second) {
							usedKeys[5].second = true;
						}
						if (usedKeys[5].second) {
							usedKeys[0].second = false;
							usedKeys[1].second = false;
							usedKeys[2].second = false;
							usedKeys[6].second = false;
							usedKeys[7].second = false;
						}
						break;
					}
					case /*usedKeys[6].first*/'1': {
						if (!usedKeys[1].second && !usedKeys[3].second && !usedKeys[5].second) {
							usedKeys[6].second = true;
						}
						if (usedKeys[6].second) {
							usedKeys[7].second = false;
						}
						break;
					}
					case /*usedKeys[7].first*/'9': {
						if (!usedKeys[1].second && !usedKeys[3].second && !usedKeys[5].second) {
							usedKeys[7].second = true;
						}
						if (usedKeys[7].second) {
							usedKeys[6].second = false;
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
			for (int j = 0; j < filenames.size(); ++j) {
				if (filenames[j] == keyFile) {
					got = true;
					break;
				}
			}
			if (!got) {
				filenames.push_back(keyFile);
			}
		}
	}
	for (int i = 0; i < usedKeys.size(); i++) {//енто тесто
		std::cout << usedKeys[i].first << "\t" << usedKeys[i].second << std::endl;
	}
	for (int i = 0; i < filenames.size(); i++) {
		std::cout << filenames[i] << "\t";
	}
	std::cout << std::endl;
	return 0;
}
