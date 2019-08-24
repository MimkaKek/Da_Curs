#ifndef MAIN_HELP_H
#define MAIN_HELP_H

#include "Globals.h"
#include "LZW.h"
#include <cstdio>
#include <cstdlib>

/* 	Имя: KeyManager
 * 	Что делает: обрабатывает полученные ключи
 */

bool KeyManager(std::string gotKeys);

/*	Имя: DifferensOfSizes
 * 	Что делает: вывод для каждого файла размера сжатого, оригинального, 
 * 	коэффициента сжатия(%) и имя ориг файла(ключ l)
 */

bool DifferensOfSizes(InBinary* file, std::string fileName);

/*	Имя: WorkWithDirectory
 * 	Что делает: работает (смотри WorkWithFile) с директорией (ключ r) 
 */
 
void WorkWithDirectory(std::string directoryName);

/*	Имя: WorkWithDirectory
 * 	Что делает: работает с файлом 
 * 	(определяет наличие файла, компресить его или декомпресить, выполняет прочие ключи)
 */

void WorkWithFile(std::string fileName);

/*	Имя: IsDirectory
 * 	Что делает: Проверяет, является ли файл директорией
 */

bool IsDirectory(std::string directoryName, bool help);

/* Имя: PrintErrors
 * Что делает: пришет что за ошибки
 */

void PrintDirectoryErrors(std::string directoryName);

/*	Имя: IsArchive
 *	Что делает: проверяет, является ли файл архивом
 */

bool IsArchive(std::string fileName);

/*	Имя: Save
 *	Что делает: Изменяет название архива на необходимое
 */

void Rename(std::string oldName, std::string nextName);

/*	Имя: Delete
 *	Что делает: Удаляет ненужный файл
 */

void Delete(std::string fileName);

/*	Имя: MainDecompress
 *	Что делает: отвечает за подготовку декомпрессинга
 */

void MainDecompress(InBinary* file, std::string fileName);

/*	Имя: MainCompress
 *	Что делает: отвечает за подготовку компрессинга
 */

void MainCompress(InBinary* file, std::string fileName);

/*	Имя: LZWCompress
 *	Что делает: подготавливает LZW компрессинг
 */

unsigned long long int LZWCompress(InBinary* file, std::string fileName,
								   OutBinary* compressionFile, int compressRatio);

/*	Имя: LZ77Compress
 *	Что делает: подготавливает LZ77 компрессинг
 */

unsigned long long int LZ77Compress(InBinary* file, std::string fileName, OutBinary* compressionFile);

/*	Имя: ArithmeticCompress
 *	Что делает: подготавливает арифметический компрессинг
 */

unsigned long long int ArithmeticCompress(InBinary* file, std::string fileName, OutBinary* compressionFile);

/*	Имя: KeepSmall
 *	Что делает: сохраняет архив самого малого размера
 */

void KeepSmall(unsigned long long int LZWSize, unsigned long long int LZ77Size,
			   unsigned long long int arithmeticSize, std::string fileName);

#endif
