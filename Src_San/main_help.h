/* main_help.h */
#ifndef MAIN_HELP_H
#define MAIN_HELP_H

#include "Globals.h"
#include "LZ77.h"
#include "Arithmetic.h"
#include <cstdio>
#include <cstdlib>

void Compress(TInBinary*, std::string);
void Decompress(TInBinary*, std::string);

bool ActivateKeys(std::string);

bool ArchiveInfo(TInBinary*, std::string);

unsigned long long int CompressA(std::string, TInBinary*);
unsigned long long int CompressL(TInBinary* file, std::string, TOutBinary*);

bool ArchiveCheck(std::string);
bool DirectoryCheck(std::string, bool);
void ChangeFile(std::string);
void GetFiles(std::string, std::map<std::string, int>*);

void SaveBest(unsigned long long int, unsigned long long int, std::string);

void ShowErrors(std::string);

void Mv(std::string, std::string);
void Rm(std::string);

#endif
