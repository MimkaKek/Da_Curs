#pragma once
//#include "Compressor.h"
#include "TPrefix.h"
#include "Globals.h"
#include <cmath>

enum Borders {
	MINIMUM_BORDER	= 	16,
	HIGH_BORDER		=	256204778801521550LLU
};

class TLZW {
public:

	/* Конструктор класса
	 * передаваемое число определяет степень сжатия или говорит о декомпрессии
	 * */
	
	TLZW(int, InBinary*, OutBinary*);

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
	InBinary* ForRead;
	
	/* основной файл для записи */
	OutBinary* ForWrite;
	
	/* Дерево компрессии */
	TPrefix* CompressionTree;

	/* дерево декомрессии */
	std::map<unsigned long long int, std::string> DecompressionTree;
	
};
