/* main_help.h */
#ifndef MAIN_HELP_H
#define MAIN_HELP_H

#include "Globals.h"
#include "LZ77.h"
#include "Arithmetic.h"
#include <cstdio>
#include <cstdlib>

void Compress(std::string);
void Decompress(std::string);

bool ActivateKeys(std::string);

void ArchiveInfo(std::string);

unsigned long long int CompressA(std::string);
unsigned long long int CompressL(std::string);

bool ArchiveCheck(std::string);
bool DirectoryCheck(std::string, bool);
void GetFiles(std::string, std::map<std::string, int>*);

void SaveBest(std::string, unsigned long long int, unsigned long long int);

void ShowErrors(std::string);

void Mv(std::string, std::string);
void Rm(std::string);

#endif
