#ifndef MAIN_HELP_H
#define MAIN_HELP_H

#include "Globals.h"
#include "LZW.h"
#include "ACC.h"
#include <cstdio>
#include <cstdlib>

bool KeyManager(std::string);

bool DifferensOfSizes(TInBinary*, std::string);
 
void WorkWithDirectory(std::string);

void WorkWithFile(std::string);

bool IsDirectory(std::string, bool);

void PrintDirectoryErrors(std::string);

bool IsArchive(std::string);

void Rename(std::string, std::string);

void Delete(std::string);

void MainDecompress(TInBinary*, std::string);

void MainCompress(TInBinary*, std::string);

unsigned long long int LZWCompress(TInBinary*, std::string, TOutBinary*);

unsigned long long int LZ77Compress(TInBinary* file, std::string, TOutBinary*);

unsigned long long int ArithmeticCompress(std::string, TInBinary*);

void KeepSmall(unsigned long long int, unsigned long long int, std::string);

#endif
