#ifndef ACC_H
#define ACC_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "Library.h"

const long BITS_IN_REGISTER = 16;
const long TOP_VALUE = ((long) 1 << 16) - 1;
const long FIRST_QTR  = (TOP_VALUE / 4) + 1;
const long HALF       = 2 * FIRST_QTR;
const long THIRD_QTR  = 3 * FIRST_QTR;
const long NO_OF_CHARS = 256;
const long EOF_SYMBOL  = NO_OF_CHARS + 1;
const long NO_OF_SYMBOLS = NO_OF_CHARS + 1;
const long MAX_FREQUENCY = 16383;

class ACC {
public:
    bool Compress (const char*, const char*);
    bool Decompress (const char*, const char*);
    ACC();
private:
    bool chError;
    unsigned char indexToChar [NO_OF_SYMBOLS];
    int charToIndex [NO_OF_CHARS];
    int cumFreq [NO_OF_SYMBOLS + 1];
    int freq [NO_OF_SYMBOLS + 1];
    long low, high;
    long value;
    long bitsToFollow;
    int buffer;
    int bitsToGo;
    int garbageBits;
    FILE *out, *in;
    void UpdateModel (int);
    void EncodeSymbol (int);
    void InputFileInfo ();
    void StartEncoding ();
    void DoneEncoding ();
    void StartDecoding ();
    int DecodeSymbol ();
    int InputBit ();
    void OutputBit (int);
    void OutputBitPlusFollow (int);
};

#endif 
