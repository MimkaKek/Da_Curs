#include "LZ77.h"
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

LZ77::LZ77()
{
	dict=(unsigned char*)calloc(dictSize_c + maxMatch_c, sizeof(char));
	hash=(unsigned int*)calloc(hashSize_c, sizeof(unsigned int));
	nextlink=(unsigned int*)calloc(dictSize_c, sizeof(unsigned int));
}
LZ77::LZ77(IStruct s):
threshold_c	(s.threshold_c),
maxCompares_c	(s.maxCompares_c),
charBits_c		(s.charBits_c),
lengthBits_c 		(s.lengthBits_c),
dictBits_c 		(s.dictBits_c),
hashBits_c 		(s.hashBits_c),
sectorBits_c 		(s.sectorBits_c),
maxMatch_c 		(s.maxMatch_c),
dictSize_c		(s.dictSize_c),	
hashSize_c 		(s.hashSize_c),
shiftBits_c		(s.shiftBits_c),
sectorSize_c		(s.sectorSize_c),
sectorAND_c		(s.sectorAND_c)
{
	dict=(unsigned char*)calloc(dictSize_c + maxMatch_c, sizeof(char));
	hash=(unsigned int*)calloc(hashSize_c, sizeof(unsigned int));
	nextlink=(unsigned int*)calloc(dictSize_c, sizeof(unsigned int));
}
void LZ77::PutBits(unsigned int bits, unsigned int numbits)
{

	bitbuf |= (bits << bitsin);

	bitsin += numbits;
	
	while (bitsin >= 8)
	{
		if (fputc(bitbuf & 0xFF, outfile) == EOF)
		{
			printf("\nerror writing to output file");
			throw std::runtime_error("Error while writing to file\n");
		}
		bitbuf >>= 8;
		bitsin -= 8;
	}

}
unsigned int LZ77::GetBits(unsigned int numbits)
{

	unsigned int i;

	i = bitbuf >> (8 - bitsin);

	while (numbits > bitsin)
	{
		if ((bitbuf = getc(infile)) == EOF)
		{
			printf("\nerror reading from input file");
			throw std::runtime_error("Error while reading from file\n");
		}
		i |= (bitbuf << bitsin);
		bitsin += 8;
	}

	bitsin -= numbits;

	return (i & masks[numbits]);

}
void LZ77::PutMatch(unsigned int matchlen, unsigned int matchdistance)
{
	PutBits(1, 1);

	PutBits(matchlen - (threshold_c + 1), lengthBits_c);

	PutBits(matchdistance, dictBits_c);
}
void LZ77::PutChar(unsigned int character)
{
	PutBits(0, 1);

	PutBits(character, charBits_c);
}
void LZ77::InitEncode()
{
	register unsigned int i;

	for (i = 0; i < hashSize_c; i++) hash[i] = NIL;

	nextlink[dictSize_c] = NIL;

}
unsigned int LZ77::LoadDict(unsigned int dictpos)
{
	register unsigned int i, j;
	
	if ((i = fread(&dict[dictpos], sizeof(char), sectorSize_c, infile)) == EOF)
	{
		printf("\nerror reading from input file");
		throw std::runtime_error("Error while loading dictionary from file\n");
	}

	if (dictpos == 0)
	{
		for (j = 0; j < maxMatch_c; j++) dict[j + dictSize_c] = dict[j];
	}

	return i;
} 
void LZ77::DeleteData(unsigned int dictpos)
{

	register unsigned int i, j;

	j = dictpos;      

	for (i = 0; i < dictSize_c; i++)
		if ((nextlink[i] & sectorAND_c) == j) nextlink[i] = NIL;

	for (i = 0; i < hashSize_c; i++)
		if ((hash[i] & sectorAND_c) == j) hash[i] = NIL;

}
void LZ77::HashData(unsigned int dictpos, unsigned int bytestodo)
{
	register unsigned int i, j, k;

	if (bytestodo <= threshold_c) 
		for (i = 0; i < bytestodo; i++) nextlink[dictpos + i] = NIL;
	else
	{
		for (i = bytestodo - threshold_c; i < bytestodo; i++)
			nextlink[dictpos + i] = NIL;
		j = (((unsigned int)dict[dictpos]) << shiftBits_c) ^ dict[dictpos + 1];
		k = dictpos + bytestodo - threshold_c;  

		for (i = dictpos; i < k; i++)
		{
			nextlink[i] = hash[j = (((j << shiftBits_c) & (hashSize_c - 1)) ^ dict[i + 2])];
			hash[j] = i;
		}
	}
}
void LZ77::FindMatch(unsigned int dictpos, unsigned int startlen)
{
	register unsigned int i, j, k;
	unsigned char l;

	i = dictpos; matchlength = startlen; k = maxCompares_c;
	l = dict[dictpos + matchlength];

	do
	{
		if ((i = nextlink[i]) == NIL) return; 

		if (dict[i + matchlength] == l)     
		{
			for (j = 0; j < maxMatch_c; j++)         
				if (dict[dictpos + j] != dict[i + j]) break;

			if (j > matchlength) 
			{
				matchlength = j;
				matchpos = i;
				if (matchlength == maxMatch_c) return; 
				l = dict[dictpos + matchlength];
			}
		}
	} while (--k); 

}
void LZ77::DictSearch(unsigned int dictpos, unsigned int bytestodo)
{

	register unsigned int i, j;

	unsigned int matchlen1, matchpos1;

	i = dictpos; j = bytestodo;

	while (j) 
	{
		FindMatch(i, threshold_c);

		if (matchlength > threshold_c)
		{
			matchlen1 = matchlength;
			matchpos1 = matchpos;

			for (; ; )
			{
				FindMatch(i + 1, matchlen1);

				if (matchlength > matchlen1)
				{
					matchlen1 = matchlength;
					matchpos1 = matchpos;
					PutChar(dict[i++]);
					j--;
				}
				else
				{
					if (matchlen1 > j)
					{
						matchlen1 = j;
						if (matchlen1 <= threshold_c) { PutChar(dict[i++]); j--; break; }
					}

					PutMatch(matchlen1, (i - matchpos1) & (dictSize_c - 1));
					i += matchlen1;
					j -= matchlen1;
					break;
				}
			}
		}
		else
		{
			PutChar(dict[i++]);
			j--;
		}
	}

}
bool LZ77::Compress(std::string in_str, std::string out_str) {
	FILE* temp_input=infile, *temp_output=outfile;
	try{
		
		if((infile = fopen(in_str.c_str(), "rb")) == NULL){
			std::cerr<<"Error: can't open read file";
			infile=temp_input, outfile=temp_output;
			return false;
		}
		if(keys[0]){
			outfile=stdout;
		}
		else{
			if((outfile = fopen(out_str.c_str(), "wb")) == NULL){
				std::cerr<<"Error: can't open write file";
				fclose(infile);
				infile=temp_input, outfile=temp_output;
				return false;
			}
		}
		char t_char='L';
		fwrite(&t_char, sizeof(char), 1, outfile);
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

		while (1)
		{
			if (deleteflag) DeleteData(dictpos);

			if ((sectorlen = LoadDict(dictpos)) == 0) break;

			HashData(dictpos, sectorlen);

			DictSearch(dictpos, sectorlen);

			bytescompressed += sectorlen;

			dictpos += sectorSize_c;

			if (dictpos == dictSize_c)
			{
				dictpos = 0;
				deleteflag = 1;  
			}
		}

		PutMatch(maxMatch_c + 1, 0);

		if (bitsin) PutBits(0, 8 - bitsin);

		if(fclose(infile)){
			std::cerr<<"Warning: input file closure failed.\n";
		}
		if(fclose(outfile)){
			std::cerr<<"Warning: output file closure failed. Data loss may occure.\n";
		}
		infile=temp_input, outfile=temp_output;

		return true;
	}
	catch(const std::exception& e){
		std::cerr << "Caught exception \"" << e.what() << "\"\n";
		fclose(infile); fclose(outfile);
		infile=temp_input, outfile=temp_output;
		return false;
	}
}
bool LZ77::Decompress(std::string in_str, std::string out_str) {
	FILE* temp_input=infile, *temp_output=outfile;
	try{
		if((infile = fopen(in_str.c_str(), "rb")) == NULL){
			std::cerr<<"Error: can't open read file";
			infile=temp_input, outfile=temp_output;
			return false;
		}
		if(!keys[5])
		{
            if(keys[0])
            {
                outfile=stdout;
            }
            else{
                if((outfile = fopen(out_str.c_str(), "wb")) == NULL)
                {
                    std::cerr<<"Error: can't open write file";
                    fclose(infile);
                    infile=temp_input, outfile=temp_output;
                    return false;
                }
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
		while(1)
		{
			if (GetBits(1) == 0)
			{
				countC++;
				dict[i++] = GetBits(charBits_c);
				if (i == dictSize_c)
				{
					if(!keys[5]){
						if (fwrite(dict, sizeof(char), dictSize_c, outfile) == EOF)
						{
							printf("\nerror writing to output file");
							throw std::runtime_error("Error while writing to output file\n");
						}
					}
					i = 0;
					bytesdecompressed += dictSize_c;
				}
			}
			else					
			{
				k = (threshold_c + 1) + GetBits(lengthBits_c);
				
				if (k == (maxMatch_c + 1))    
				{
					if(!keys[5]){
						if (fwrite(dict, sizeof(char), i, outfile) == EOF)
						{
							printf("\nerror writing to output file");
							throw std::runtime_error("Error while writing to output file\n");
						}
					}
					bytesdecompressed += i;
					return true;
				}
				countM++;
				countL+=k;
				j = ((i - GetBits(dictBits_c)) & (dictSize_c - 1));
				
				do
				{
					dict[i++] = dict[j++];
					j &= (dictSize_c - 1);
					if (i == dictSize_c)
					{
						if(!keys[5]){
							if (fwrite(dict, sizeof(char), dictSize_c, outfile) == EOF)
							{
								printf("\nerror writing to output file");
								throw std::runtime_error("Error while writing to output file\n");
							}
						}
						i = 0;
						bytesdecompressed += dictSize_c;
					}
				} while (--k);
			}
		}
	}
	catch(const std::exception& e){
		std::cerr << "Caught exception \"" << e.what() << "\"\n";
		fclose(infile); fclose(outfile);
		infile=temp_input, outfile=temp_output;
		return false;
	}
}
