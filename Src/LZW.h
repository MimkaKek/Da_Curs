/* LZW.h */
#pragma once
#include "TPrefix.h"
#include "Globals.h"
#include <cmath>

const short int MINIMUM_BORDER = 16;

class TLZW {
public:
	
	TLZW(int, TInBinary*, TOutBinary*);
	
	bool Compress(std::string);
	
	bool Decompress(std::string);

	~TLZW();

private:
	
	TInBinary* ForRead;
	
	TOutBinary* ForWrite;
	
	TPrefix* CompressionTree;

	std::map<unsigned long long int, std::string> DecompressionTree;
};
