#include "main_help.h"

/* ключи Олега и Сани																	как будут представлены
 * -c вывод архива в out																функция
 * -d распаковка																		bool
 * -k не удалять файлы после компрессии или декомпрессии								bool
 * -l вывод для каждого файла размера сжатого, оригинального,
  	  коэффициента сжатия(%) и имя ориг файла											функция
 * -r рекурсивно разобрать дерево каталогов (сжатие/разжатие всех файлов в папке)		функция
 * -t проверить целостность сжатого файла												функция
 * -1 быстро сжать																		bool/int
 * -9 максимум сжатия																	bool/int
 */
 
//	У Славы вроде есть ещё какие-то свои ключи 
 
//	gzip-у пофиг на повторяющиеся ключи и даже всё равно если одновременно юзать -1 и -9	\^_^/

//	но компресить и декомпресить за 1 раз он не могёт
//	t перекрывает c и недает ему работать и не работает при компрессии
//	l перекрывает t и d и недает ему работать и не работает при компрессии
//	d и c вместе не работают																/Т_Т\

int main(int argc, char *argv[]) {
	std::vector<std::string> filenames;
	std::vector<char> keys = {'c', 'd', 'k', 'l', 'r', 't', '1', '9'};
	std::vector<bool> usedKeys = {false, false, false, false, false, false, false, false};
	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {//ага ключ(и)
			for (int j = 0; j < keys.size(); ++j) {
				for (int k = 0; k < keys.size(); ++k) {
					if (keys[k] == argv[i][j + 1]) {
						usedKeys[j] = true;
						break;
					}
				}
			}
		}
		else {// ага файлы
			bool got = false;
			for (int j = 0; j < filenames.size(); ++j) {
				if (filenames[j] == argv[i]) {
					got = true;
					break;
				}
			}
			if (!got) {
				filenames.push_back(argv[i]);
			}
		}
	}
	for (int i = 0; i < keys.size(); i++) {//енто тесто
		std::cout << keys[i] << "\t" << usedKeys[i] << std::endl;
	}
	for (int i = 0; i < filenames.size(); i++) {
		std::cout << filenames[i] << std::endl;
	}
	/*if (usedKeys[6] && usedKeys[7]) {
		std::cout << "биполярОчка" << std::endl;
	}*/
	
	return 0;
}
