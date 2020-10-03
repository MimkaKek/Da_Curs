/* LZW.h */
#pragma once
#include "TPrefix.h"
#include "Globals.h"

class TLZW {
public:
	
	TLZW(TInBinary*, TOutBinary*);
	
	bool Compress(std::string);
	
	bool Decompress(std::string);

	~TLZW();

private:
	
	TInBinary* ForRead;
	
	TOutBinary* ForWrite;
	
	TPrefix* CompressionTree;

	std::map<unsigned long long int, std::string> DecompressionTree;
};
