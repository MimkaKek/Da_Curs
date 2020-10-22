#ifndef PREFIX_H
#define PREFIX_H

#include "Globals.h"
#include "BFile.h"
#include <limits>

const short int CHAR_HAS = 256;

enum UpdateResult {
    GOT_EOF,
    OK,
    FULL,
    MEMORY_ERROR,
    WRITE_ERROR,
    READ_ERROR
};

class TPrefix {
public:

	TPrefix(TInBinary*, TOutBinary*);

	TPrefix();

	int Update(unsigned char);

	int UpdateForRoot();

	void Clear(bool);

	~TPrefix();

private:

	std::vector<std::pair<unsigned char, TPrefix*>> Next;

	unsigned long long int NumberOfWord;

	static unsigned char LastLetter;

	static unsigned long long int NeedToRead;

	static unsigned long long int LastNumber;

	static unsigned long long int Border;
	
	static TInBinary* ForRead; 

	static TOutBinary* ForWrite;

	static unsigned short int Bites;
};

#endif
