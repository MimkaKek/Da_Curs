#ifndef MAIN_HELP_H
#define MAIN_HELP_H

#include "Globals.h"
#include "LZ77.h"
#include "ACC.h"
#include <cstdio>
#include <cstdlib>
#include <string>

void FileIterator(std::map<std::string, int>);

bool Parser(std::map<std::string, int>*, std::string);

bool AskDir(std::string, bool);

void DirectoryWork(std::string);

void DeComPress(std::string);

bool Rewrite(std::string);

void ErrorNotes(std::string);

bool KeyL(TInBinary*, std::string);

void PreCompress(TInBinary*, std::string);

unsigned long long int Compress(std::string, TInBinary*, bool);

void PreDecompress(TInBinary*, std::string);

#endif
