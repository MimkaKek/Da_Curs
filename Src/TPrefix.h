/* TPrefix.h */
#ifndef TPREFIX_H
#define TPREFIX_H

#include "Globals.h"
#include "BFile.h"
#include <limits>

class TPrefix {
public:
	TPrefix(unsigned long long int, TInBinary*, TOutBinary*);

	TPrefix();

	int Update(char);

	int UpdateForRoot();

	void Clear(bool);

	~TPrefix();

private:

	std::vector<std::pair<char, TPrefix*>> Next;

	unsigned long long int NumberOfWord;

	static char LastLetter;

	static unsigned long long int NeedToRead;

	static unsigned long long int LastNumber;

	static unsigned long long int Border;
	
	static TInBinary* ForRead; 

	static TOutBinary* ForWrite;

	static unsigned short int Bites;
};

#endif
