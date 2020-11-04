/* LZ77.h */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <bitset>
#include <string>
#include "Library.h"

class Compressor;


// template <const int compressFloor = 2, const bool GREEDY = false, const int MAXCOMPARES = 75, const int CHARBITS = 8 , const int MATCHBITS = 4, const int DICTBITS = 13, const int HASHBITS = 10,
//  const int SECTORBITS = 10>
class TLZ77
{

public:

	typedef struct InitStruct{
		const int compressFloor 		= 2;
		const int comparesCeil 			= 75;
		const int CHARBITS				= 8;
		const int MATCHBITS 			= 4; 
		const int DICTBITS 				= 13;
		const int HASHBITS 				= 10;
		const int SECTORBITS 			= 10;
		const unsigned int MAXMATCH 	= (1 << MATCHBITS) + compressFloor - 1;
		const unsigned int DICTSIZE		= 1 << DICTBITS;
		const unsigned int HASHSIZE 	= 1 << HASHBITS;
		const unsigned int SHIFTBITS	= (HASHBITS + compressFloor) / (compressFloor + 1);
		const unsigned int SECTORLEN	= 1 << SECTORBITS;
		const unsigned int SECTORAND	= (0xFFFF << SECTORBITS) & 0xFFFF;
	} IStruct;
	TLZ77();
	TLZ77(IStruct s){
		dict=(unsigned char*)calloc(DICTSIZE + MAXMATCH, sizeof(char));
		hash=(unsigned int*)calloc(HASHSIZE, sizeof(unsigned int));
		nextlink=(unsigned int*)calloc(DICTSIZE, sizeof(unsigned int));
	};
	void InitEncode();

	void Compress();
	bool Compress(std::string in_str, std::string out_str);
	void Decompress();
	bool Decompress(std::string in_str, std::string out_str);
	unsigned int LoadDict(unsigned int dictpos);
	void DeleteData(unsigned int dictpos);
	void HashData(unsigned int dictpos, unsigned int bytestodo);
	void FindMatch(unsigned int dictpos, unsigned int startlen);
	void DictSearch(unsigned int dictpos, unsigned int bytestodo);

	void SendChar(unsigned int character);
	void SendMatch(unsigned int matchlen, unsigned int matchdistance);
	
	unsigned int ReadBits(unsigned int numbits);
	void SendBits(unsigned int bits, unsigned int numbits);
	virtual ~TLZ77(){free(dict);free(hash);free(nextlink);};
//private:
	const int compressFloor 		= 2;
	const int comparesCeil 			= 75;
	const int CHARBITS				= 8;
	const int MATCHBITS 			= 4; 
	const int DICTBITS 				= 13;
	const int HASHBITS 				= 10;
	const int SECTORBITS 			= 10;
	const unsigned int MAXMATCH 	= (1 << MATCHBITS) + compressFloor - 1;
	const unsigned int DICTSIZE		= 1 << DICTBITS;
	const unsigned int HASHSIZE 	= 1 << HASHBITS;
	const unsigned int SHIFTBITS	= (HASHBITS + compressFloor) / (compressFloor + 1);
	const unsigned int SECTORLEN	= 1 << SECTORBITS;
	const unsigned int SECTORAND	= (0xFFFF << SECTORBITS) & 0xFFFF;
	/* dictionary plus MAXMATCH extra chars for string comparisions */
	unsigned char* dict;
	/* hashtable & link list table */
	unsigned int *hash, *nextlink;
	/* misc. global variables */
	unsigned int
		counter=0,
		matchlength=0,
		matchpos=0,
		bitbuf=0,
		bitsin=0,
		masks[17] = { 0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535 };

		FILE *infile, *outfile;
};
