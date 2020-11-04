#include "LZ77.h"
#include <exception>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <cstdio>

#define KB	1024
#define MB	1024*KB
#define NIL 0xFFFF
#define REM(x,y) ((double)((x)%(y)))/(y)

TLZ77::TLZ77() {
	dict = (unsigned char*)calloc(DICTSIZE + MAXMATCH, sizeof(char));
	hash = (unsigned int*)calloc(HASHSIZE, sizeof(unsigned int));
	nextlink = (unsigned int*)calloc(DICTSIZE, sizeof(unsigned int));
}

void TLZ77::SendBits(unsigned int bits, unsigned int numbits) {
	bitbuf |= (bits << bitsin);
	bitsin += numbits;
	while (bitsin >= 8) {
		if (fputc(bitbuf & 0xFF, outfile) == EOF) {
			printf("\nerror writing to output file");
			throw std::runtime_error("Error while writing to file\n");
		}
		bitbuf >>= 8;
		bitsin -= 8;
		counter++;
	}
	return;
}

unsigned int TLZ77::ReadBits(unsigned int numbits) {
	unsigned int i;
	i = bitbuf >> (8 - bitsin);
	while (numbits > bitsin) {
		if ((bitbuf = getc(infile)) == EOF) {
			printf("\nerror reading from input file");
			throw std::runtime_error("Error while reading from file\n");
		}
		i |= (bitbuf << bitsin);
		bitsin += 8;
	}
	bitsin -= numbits;
	return (i & masks[numbits]);
}

void TLZ77::SendMatch(unsigned int matchlen, unsigned int matchdistance) {
	SendBits(1, 1);
	SendBits(matchlen - (compressFloor + 1), MATCHBITS);
	SendBits(matchdistance, DICTBITS);
	return;
}

void TLZ77::SendChar(unsigned int character) {
	SendBits(0, 1);
	SendBits(character, CHARBITS);
	return;
}

void TLZ77::InitEncode() {
	register unsigned int i;
	for (i = 0; i < HASHSIZE; i++)
		hash[i] = NIL;
	nextlink[DICTSIZE] = NIL;
	return;
}

unsigned int TLZ77::LoadDict(unsigned int dictpos) {
	register unsigned int i, j;
	if ((i = fread(&dict[dictpos], sizeof(char), SECTORLEN, infile)) == EOF) {
		printf("\nerror reading from input file");
		throw std::runtime_error("Error while loading dictionary from file\n");
	}
	if (dictpos == 0) {
		for (j = 0; j < MAXMATCH; j++)
			dict[j + DICTSIZE] = dict[j];
	}
	return i;
} 

void TLZ77::DeleteData(unsigned int dictpos) {
	register unsigned int i, j;
	j = dictpos;
	for (i = 0; i < DICTSIZE; i++)
		if ((nextlink[i] & SECTORAND) == j)
			nextlink[i] = NIL;
	for (i = 0; i < HASHSIZE; i++)
		if ((hash[i] & SECTORAND) == j)
			hash[i] = NIL;
	return;
}

void TLZ77::HashData(unsigned int dictpos, unsigned int bytestodo) {
	register unsigned int i, j, k;
	if (bytestodo <= compressFloor)   
		for (i = 0; i < bytestodo; i++)
			nextlink[dictpos + i] = NIL;
	else {
		for (i = bytestodo - compressFloor; i < bytestodo; i++)
			nextlink[dictpos + i] = NIL;
		j = (((unsigned int)dict[dictpos]) << SHIFTBITS) ^ dict[dictpos + 1];
		k = dictpos + bytestodo - compressFloor;  

		for (i = dictpos; i < k; i++) {
			nextlink[i] = hash[j = (((j << SHIFTBITS) & (HASHSIZE - 1)) ^ dict[i + 2])];
			hash[j] = i;
		}
	}
	return;
}

void TLZ77::FindMatch(unsigned int dictpos, unsigned int startlen) {
	register unsigned int i, j, k;
	unsigned char l;
	i = dictpos; matchlength = startlen; k = comparesCeil;
	l = dict[dictpos + matchlength];
	do {
		if ((i = nextlink[i]) == NIL)
			return;   
		if (dict[i + matchlength] == l) {
			for (j = 0; j < MAXMATCH; j++)          
				if (dict[dictpos + j] != dict[i + j])
					break;
			if (j > matchlength) {
				matchlength = j;
				matchpos = i;
				if (matchlength == MAXMATCH)
					return;  
				l = dict[dictpos + matchlength];
			}
		}
	} while (--k);
	return;
}

void TLZ77::DictSearch(unsigned int dictpos, unsigned int bytestodo) {
	register unsigned int i, j;
	i = dictpos; j = bytestodo;
	while (j) {
		FindMatch(i, compressFloor);
		if (matchlength > j)
			matchlength = j;
		if (matchlength > compressFloor) {
			SendMatch(matchlength, (i - matchpos) & (DICTSIZE - 1));
			i += matchlength;
			j -= matchlength;
		}
		else {
			SendChar(dict[i]);
			++i;
			j--;
		}
	}
	return;
}

bool TLZ77::Compress(std::string in_str, std::string out_str) {
	FILE* temp_input = infile, *temp_output = outfile;
	try {		
		if ((infile = fopen(in_str.c_str(), "rb")) == NULL) {
			std::cerr<<"Error: can't open read file";
			infile = temp_input, outfile = temp_output;
			return false;
		}
		if (keys[0]) {
			outfile = stdout;
		}
		else {
			if ((outfile = fopen(out_str.c_str(), "wb")) == NULL) {
				std::cerr<<"Error: can't open write file";
				fclose(infile);
				infile = temp_input, outfile = temp_output;
				return false;
			}
		}
		char tmp_char = 'L';
		fwrite(&tmp_char, sizeof(char), 1, outfile);
		unsigned long long savePos, sizeOfFile;
		savePos = ftell(infile);
		fseek(infile, 0, SEEK_END);
		sizeOfFile = ftell(infile);
		fseek(infile, savePos, SEEK_SET);
		fwrite(&sizeOfFile, sizeof(long long), 1, outfile);
		unsigned int dictpos, deleteflag, sectorlen;
		unsigned long bytescompressed;
		InitEncode();
		dictpos = deleteflag = 0;
		bytescompressed = 0;
		while (1) {
			if (deleteflag)
				DeleteData(dictpos);
			if ((sectorlen = LoadDict(dictpos)) == 0)
				break;
			HashData(dictpos, sectorlen);
			DictSearch(dictpos, sectorlen);
			bytescompressed += sectorlen;
			dictpos += SECTORLEN;
			if (dictpos == DICTSIZE) {
				dictpos = 0;
				deleteflag = 1;
			}
		}
		SendMatch(MAXMATCH + 1, 0);
		if (bitsin)
			SendBits(0, 8 - bitsin);
		if (fclose(infile)) {
			std::cerr << "Warning: input file closure failed.\n";
		}
		if (fclose(outfile)) {
			std::cerr << "Warning: output file closure failed. Data loss may occure.\n";
		}
		infile = temp_input, outfile = temp_output;
		return true;
	}
	catch(const std::exception& e) {
		std::cerr << "Caught exception \"" << e.what() << "\"\n";
		fclose(infile);
		fclose(outfile);
		infile = temp_input, outfile = temp_output;
		return false;
	}
}

bool TLZ77::Decompress(std::string in_str, std::string out_str) {
	FILE* temp_input = infile, *temp_output = outfile;
	try {
		if ((infile = fopen(in_str.c_str(), "rb")) == NULL) {
			std::cerr << "Error: can't open read file";
			infile = temp_input, outfile = temp_output;
			return false;
		}
		if (!keys[5]) {
			if ((outfile = fopen(out_str.c_str(), "wb")) == NULL) {
				std::cerr << "Error: can't open write file";
				fclose(infile);
				infile = temp_input, outfile = temp_output;
				return false;
			}
		}
		char typeC = 0;
		unsigned long long oldSize = 0;
		fread(&typeC, sizeof(char), 1, infile);
		fread(&oldSize, sizeof(long long), 1, infile);
		register unsigned int i, j, k;
		unsigned long bytesdecompressed;
		i = 0;
		bytesdecompressed = 0;
		int64_t countC=0,countM=0,countL=0;
		for (;;) {
			if (ReadBits(1) == 0) {
				countC++;
				dict[i++] = ReadBits(CHARBITS);
				if (i == DICTSIZE) {
					if (!keys[5]) {
						if (fwrite(dict, sizeof(char), DICTSIZE, outfile) == EOF) {
							printf("\nerror writing to output file");
							throw std::runtime_error("Error while writing to output file\n");
						}
					}
					i = 0;
					bytesdecompressed += DICTSIZE;
				}
			}
			else {
				k = (compressFloor + 1) + ReadBits(MATCHBITS);
				if (k == (MAXMATCH + 1)) {
					if (!keys[5]) {
						if (fwrite(dict, sizeof(char), i, outfile) == EOF) {
							printf("\nerror writing to output file");
							throw std::runtime_error("Error while writing to output file\n");
						}
					}
					bytesdecompressed += i;
					return true;
				}
				countM++;
				countL += k;
				j = ((i - ReadBits(DICTBITS)) & (DICTSIZE - 1));
				do {
					dict[i++] = dict[j++];
					j &= (DICTSIZE - 1);
					if (i == DICTSIZE) {
						if (!keys[5]) {
							if (fwrite(dict, sizeof(char), DICTSIZE, outfile) == EOF) {
								printf("\nerror writing to output file");
								throw std::runtime_error("Error while writing to output file\n");
							}
						}
						i = 0;
						bytesdecompressed += DICTSIZE;
					}
				} while (--k);
			}
		}
	}
	catch(const std::exception& e) {
		std::cerr << "Caught exception \"" << e.what() << "\"\n";
		fclose(infile);
		fclose(outfile);
		infile = temp_input, outfile = temp_output;
		return false;
	}
}
