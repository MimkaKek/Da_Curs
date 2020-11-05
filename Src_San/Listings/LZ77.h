#pragma once
#include "Globals.h"
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

class LZ77
{

public:

	typedef struct InitStruct{
		int threshold_c 		= 2;
		int maxCompares_c 		= 75;
		int lengthBits_c 		= 4; 
		int dictBits_c 			= 13;
		int hashBits_c 			= 10;
		int sectorBits_c 		= 10;
		int charBits_c			= 8;
		unsigned int maxMatch_c 	= (1 << lengthBits_c) + threshold_c - 1;
		unsigned int dictSize_c	= 1 << dictBits_c;
		unsigned int hashSize_c 	= 1 << hashBits_c;
		unsigned int shiftBits_c	= (hashBits_c + threshold_c) / (threshold_c + 1);
		unsigned int sectorSize_c	= 1 << sectorBits_c;
		unsigned int sectorAND_c	= (0xFFFF << sectorBits_c) & 0xFFFF;
	} IStruct;
	LZ77();
	LZ77(IStruct s);
	void InitEncode();
	bool Compress(std::string in_str, std::string out_str);
	bool Decompress(std::string in_str, std::string out_str);
	
	virtual ~LZ77(){free(dict);free(hash);free(nextlink);};
private:
	unsigned int LoadDict(unsigned int dictpos);
	void DeleteData(unsigned int dictpos);
	void HashData(unsigned int dictpos, unsigned int bytestodo);
	void FindMatch(unsigned int dictpos, unsigned int startlen);
	void DictSearch(unsigned int dictpos, unsigned int bytestodo);

	void PutChar(unsigned int character);
	void PutMatch(unsigned int matchlen, unsigned int matchdistance);
	
	unsigned int GetBits(unsigned int numbits);
	void PutBits(unsigned int bits, unsigned int numbits);

	const int threshold_c 		= 2;
	const int maxCompares_c 			= 75;
	const int charBits_c				= 8;
	const int lengthBits_c 			= 4; 
	const int dictBits_c 				= 13;
	const int hashBits_c 				= 10;
	const int sectorBits_c 			= 10;
	const unsigned int maxMatch_c 	= (1 << lengthBits_c) + threshold_c - 1;
	const unsigned int dictSize_c		= 1 << dictBits_c;
	const unsigned int hashSize_c 	= 1 << hashBits_c;
	const unsigned int shiftBits_c	= (hashBits_c + threshold_c) / (threshold_c + 1);
	const unsigned int sectorSize_c	= 1 << sectorBits_c;
	const unsigned int sectorAND_c	= (0xFFFF << sectorBits_c) & 0xFFFF;
	unsigned char* dict;
	unsigned int *hash, *nextlink;
	unsigned int
		matchlength=0,
		matchpos=0,
		bitbuf=0,
		bitsin=0,
		masks[17] = { 0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535 };

		FILE *infile, *outfile;
};
