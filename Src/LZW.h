#pragma once
//#include "Compressor.h"
#include "TPrefix.h"
#include "Globals.h"

const std::map<unsigned long long int, std::string> ONLY_FOR_ENUMS;


/*
enum Borders {
	LOW_BORDER		=	10000,
	MEDIUM_BORDER	=	100000000000,
	HIGH_BORDER		=	256204778801521550
};*/

enum Borders {
	LOW_BORDER		=	10000,
	MEDIUM_BORDER	=	100000000000LLU,
	HIGH_BORDER		=	256204778801521550LLU
};

enum VariousDividers {
	LOW_FAST 		= 	100, 
	LOW_MEDIUM		= 	10,
	MEDIUM_FAST		= 	1000000,
	MEDIUM_MEDIUM	= 	1000,
	HIGH_FAST		= 	MEDIUM_BORDER * 10,
	HIGH_MEDIUM		= 	LOW_BORDER * 10
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
