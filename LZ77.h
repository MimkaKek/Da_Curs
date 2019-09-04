#pragma once
#include "Compressor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>

#define KB  1024
#define MB  1024 * KB

/* set this to 1 for a greedy encoder */
#define GREEDY    0

/* ratio vs. speed constant */
/* the larger this constant, the better the compression */
#define MAXCOMPARES 75

/* unused entry flag */
#define NIL       0xFFFF

/* bits per symbol- normally 8 for general purpose compression */
#define CHARBITS  8

/* minimum match length & maximum match length */
#define THRESHOLD 2
#define MATCHBITS 4
#define MAXMATCH  ((1 << MATCHBITS) + THRESHOLD - 1)

/* sliding dictionary size and hash table's size */
/* some combinations of HASHBITS and THRESHOLD values will not work
		 correctly because of the way this program hashes strings */
#define DICTBITS  13
#define HASHBITS  10
#define DICTSIZE  (1 << DICTBITS)
#define HASHSIZE  (1 << HASHBITS)

		 /* # bits to shift after each XOR hash */
		 /* this constant must be high enough so that only THRESHOLD + 1
				  characters are in the hash accumulator at one time */
#define SHIFTBITS ((HASHBITS + THRESHOLD) / (THRESHOLD + 1))

				  /* sector size constants */
#define SECTORBIT 10
#define SECTORLEN (1 << SECTORBIT)
#define SECTORAND ((0xFFFF << SECTORBIT) & 0xFFFF)

class LZ77: public Compressor
{
public:
	LZ77();
	void Compress3();
	/* 
	 * Что делает: Сжимает данные
	 */
	void Compress() override;
	void Compress2();
	/* 
	 * Что делает: Сжимает данные
	 */
	void Decompress() override;

	/* 
	 * Что делает: Инициализирует поисковую структуру(словарь) необходимую для сжатия
	 */
	void InitEncode();

	/* 
	 * Что делает: Загружает словарь из потока ввода, начиная с позиции dictpos
	 */
	unsigned int LoadDict(unsigned int dictpos);

	/*
	Что делает: Удаляет из словаря сектор dectpos
	*/
	void DeleteData(unsigned int dictpos);

	/*
	Что делает: Хэширует данные
	 */
	void HashData(unsigned int dictpos, unsigned int bytestodo);

	/*
	 * Что делает: Ищет в словаре совпадение со строкой начиная с позиции dictpos
	 */
	void FindMatch(unsigned int dictpos, unsigned int startlen);

	/*
	 * Что делает: Ищет в словаре совпадение символов для текущего сектора
	 */
	void DictSearch(unsigned int dictpos, unsigned int bytestodo);
	void DictSearch2(unsigned int dictpos, unsigned int bytestodo, std::string text);

	void SendChar(unsigned int character);
	void SendMatch(unsigned int matchlen, unsigned int matchdistance);
	unsigned int ReadBits(unsigned int numbits);
	void SendBits(unsigned int bits, unsigned int numbits);
	virtual ~LZ77();
//private:
	/* dictionary plus MAXMATCH extra chars for string comparisions */
	unsigned char
		dict[DICTSIZE + MAXMATCH];

	/* hashtable & link list table */
	unsigned int
		hash[HASHSIZE],
		nextlink[DICTSIZE];

	/* misc. global variables */
	unsigned int
		counter=0,
		matchlength,
		matchpos,
		bitbuf,
		bitsin,
		masks[17] = { 0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535 };
	
	FILE *infile, *outfile;

	
};

