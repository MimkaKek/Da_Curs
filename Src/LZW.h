#pragma once
#include "TPrefix.h"
#include "Globals.h"
#include <cmath>

const short int MINIMUM_BORDER = 16;

class TLZW {
public:

	/* Конструктор класса
	 * передаваемое число определяет степень сжатия или говорит о декомпрессии
	 * */
	
	TLZW(int, TInBinary*, TOutBinary*);

	/*	Имя: Compress
	 *	Что делает: Принимает решение о степени сжатия
	 */
	
	bool Compress(std::string);

	/*	Имя: Decompress
	 *	Что делает: Расжимает данные
	 */
	
	bool Decompress(std::string fileName);

	~TLZW();

private:
	
	/* файл из которого осуществляется считывание */
	TInBinary* ForRead;
	
	/* основной файл для записи */
	TOutBinary* ForWrite;
	
	/* Дерево компрессии */
	TPrefix* CompressionTree;

	/* дерево декомрессии */
	std::map<unsigned long long int, std::string> DecompressionTree;
};
