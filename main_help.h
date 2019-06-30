#ifndef MAIN_HELP_H
#define MAIN_HELP_H


#include "Globals.h"

/*	Имя: ShowArc
 * 	Что делает: вывод архива в out (ключ c)
 */

void ShowArc(std::string archiveName);

/*	Имя: DifferensOfSizes
 * 	Что делает: вывод для каждого файла размера сжатого, оригинального, 
 * 	коэффициента сжатия(%) и имя ориг файла(ключ l)
 */

void DifferensOfSizes(std::string archiveName);

/*	Имя: WorkWithDirectory
 * 	Что делает: работает (смотри WorkWithFile) с директорией (ключ r) 
 */
 
void WorkWithDirectory(std::string directoryName, std::vector<bool> keys);

/*	Имя: WorkWithDirectory
 * 	Что делает: работает с файлом 
 * 	(определяет наличие файла, компресить его или декомпресить, выполняет прочие ключи)
 */

void WorkWithFile(std::string fileName, std::vector<bool> keys);

/*	Имя: CheckIntegrity
 * 	Что делает: Проверяет целостность сжатого файла (ключ t)
 * 	в случае повреждения файла выводит соответствующее сообщение иначе молчит
 */

void CheckIntegrity(std::string archiveName);

/*	Имя: IsDirectory
 * 	Что делает: Проверяет, является ли файл директорией
 */

bool IsDirectory(std::string directoryName);

/*	Имя: IsArchive
 *	Что делает: проверяет, является ли файл архивом
 */

bool IsArchive(std::string fileName);

#endif
