#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <errno.h>
#include <map>
#include <ctime>//TEST

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

/*	A - Арифметическое
 *	7 - LZ77
 * 	W - LZW
 */

/* Структура архива: 1)буква помогающая определить что за алгоритм это кодировал (смотри выше)
 * 					 2)Размер несжатого файла
 * 					 3)То что надо вашим алгоритмам
 */
//typedef std;//TEST это для меня, потом удалю

extern std::vector<bool> keys;

const int CHAR_HAS = 256;

enum CompressionRatio {
	FAST,
	NORMAL,
	HIGH,
	DECOMPRESS
};

enum Sizeof {
	LLINT = sizeof(unsigned long long int),
	CHAR = sizeof(char)
};

enum UpdateResult {
	GOT_EOF,
	OK,
	FULL,
	MEMORY_ERROR,
	WRITE_ERROR
};

#endif