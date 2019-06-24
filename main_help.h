#ifndef MAIN_HELP_H
#define MAIN_HELP_H

#include <iostream>
#include <sstream>
#include <vector>
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

/*	Имя: GetFiles
 * 	Что делает: получает вектор файлов из заданной дирректории, которые надо компрессить/декомпрессить (ключ r)
 */
 
std::vector<std::string> GetFiles(std::string directory);

/*	Имя: CheckIntegrity
 * 	Что делает: Проверяет целостность сжатого файла (ключ t)
 * 	в случае повреждения файла выводит соответствующее сообщение иначе молчит
 */

void CheckIntegrity(std::string archiveName);

#endif
