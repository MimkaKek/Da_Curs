/* LZ77.h */
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
#include <bitset>
#include <string>


class Compressor;


// template <const int compressFloor = 2, const bool GREEDY = false, const int MAXCOMPARES = 75, const int CHARBITS = 8 , const int MATCHBITS = 4, const int DICTBITS = 13, const int HASHBITS = 10,
//  const int SECTORBITS = 10>
class LZ77: public Compressor
{

public:

	typedef struct InitStruct{
		int comparesCeil 		= 75;
		int compressFloor 		= 2;
		int MATCHBITS 			= 4; 
		int DICTBITS 			= 13;
		int HASHBITS 			= 10;
		int SECTORBITS 			= 10;
		int CHARBITS			= 8;
		unsigned int MAXMATCH 	= (1 << MATCHBITS) + compressFloor - 1;
		unsigned int DICTSIZE	= 1 << DICTBITS;
		unsigned int HASHSIZE 	= 1 << HASHBITS;
		unsigned int SHIFTBITS	= (HASHBITS + compressFloor) / (compressFloor + 1);
		unsigned int SECTORLEN	= 1 << SECTORBITS;
		unsigned int SECTORAND	= (0xFFFF << SECTORBITS) & 0xFFFF;
	} IStruct;
	LZ77();
	LZ77(IStruct s);
	void InitEncode();

	void Compress() override;
	bool Compress(std::string in_str, std::string out_str);
	void Decompress() override;
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
	virtual ~LZ77(){free(dict);free(hash);free(nextlink);};
private:
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