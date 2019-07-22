#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <errno.h>

/* ключи																	
 * -c вывод архива в out
 * -d распаковка
 * -k не удалять файлы после компрессии или декомпрессии
 * -l вывод для каждого файла размера сжатого, оригинального, коэффициента сжатия(%) и имя ориг файла
 * -r рекурсивно разобрать дерево каталогов (сжатие/разжатие всех файлов в папке но сама папка нетрогается)
 * -t проверить целостность сжатого файла
 * -1 быстро сжать
 * -9 максимум сжатия
 */

typedef std;//TEST это для меня, потом удалю

extern std::vector<bool> keys;

const int ASCII_SIMBOLS = 128;

enum CompressionRatio {//Пусть пока будет
	FAST,
	NORMAL,
	HIGH,
	DECOMPRESS
};

#endif
