#include "LZ77.h"
#include "LZ77.h"

#define KB	1024
#define MB	1024*KB
#define NIL 0xFFFF
#define REM(x,y) ((double)((x)%(y)))/(y)

LZ77::LZ77()
{
	dict=(unsigned char*)calloc(DICTSIZE + MAXMATCH, sizeof(char));
	hash=(unsigned int*)calloc(HASHSIZE, sizeof(unsigned int));
	nextlink=(unsigned int*)calloc(DICTSIZE, sizeof(unsigned int));
}
/* writes multiple bit codes to the output stream */
void LZ77::SendBits(unsigned int bits, unsigned int numbits)
{

	bitbuf |= (bits << bitsin);

	bitsin += numbits;
	
	while (bitsin >= 8)
	{
		if (fputc(bitbuf & 0xFF, outfile) == EOF)
		{
			printf("\nerror writing to output file");
			exit(EXIT_FAILURE);
		}
		bitbuf >>= 8;
		bitsin -= 8;
		counter++;
	}

}
/* reads multiple bit codes from the input stream */
unsigned int LZ77::ReadBits(unsigned int numbits)
{

	unsigned int i;

	i = bitbuf >> (8 - bitsin);

	while (numbits > bitsin)
	{
		if ((bitbuf = getc(infile)) == EOF)
		{
			printf("\nerror reading from input file");
			exit(EXIT_FAILURE);
		}
		i |= (bitbuf << bitsin);
		bitsin += 8;
	}

	bitsin -= numbits;

	return (i & masks[numbits]);

}
/* sends a match to the output stream */
void LZ77::SendMatch(unsigned int matchlen, unsigned int matchdistance)
{
	SendBits(1, 1);

	SendBits(matchlen - (compressFloor + 1), MATCHBITS);

	SendBits(matchdistance, DICTBITS);
}
/* sends one character (or literal) to the output stream */
void LZ77::SendChar(unsigned int character)
{
	SendBits(0, 1);

	SendBits(character, CHARBITS);
}
void LZ77::InitEncode()
{
	register unsigned int i;

	for (i = 0; i < HASHSIZE; i++) hash[i] = NIL;

	nextlink[DICTSIZE] = NIL;

}
/* loads dictionary with characters from the input stream */
unsigned int LZ77::LoadDict(unsigned int dictpos)
{
	register unsigned int i, j;
	
	if ((i = fread(&dict[dictpos], sizeof(char), SECTORLEN, infile)) == EOF)
	{
		printf("\nerror reading from input file");
		exit(EXIT_FAILURE);
	}

	/* since the dictionary is a ring buffer, copy the characters at
			 the very start of the dictionary to the end */
	if (dictpos == 0)
	{
		for (j = 0; j < MAXMATCH; j++) dict[j + DICTSIZE] = dict[j];
	}

	return i;
}
/* deletes data from the dictionary search structures */
/* this is only done when the number of bytes to be
		 compressed exceeds the dictionary's size */
void LZ77::DeleteData(unsigned int dictpos)
{

	register unsigned int i, j;

	j = dictpos;        /* put dictpos in register for more speed */

	/* delete all references to the sector being deleted */

	for (i = 0; i < DICTSIZE; i++)
		if ((nextlink[i] & SECTORAND) == j) nextlink[i] = NIL;

	for (i = 0; i < HASHSIZE; i++)
		if ((hash[i] & SECTORAND) == j) hash[i] = NIL;

}
/* hash data just entered into dictionary */
/* XOR hashing is used here, but practically any hash function will work */
void LZ77::HashData(unsigned int dictpos, unsigned int bytestodo)
{
	register unsigned int i, j, k;

	if (bytestodo <= compressFloor)   /* not enough bytes in sector for match? */
		for (i = 0; i < bytestodo; i++) nextlink[dictpos + i] = NIL;
	else
	{
		/* matches can't cross sector boundries */
		for (i = bytestodo - compressFloor; i < bytestodo; i++)
			nextlink[dictpos + i] = NIL;

		j = (((unsigned int)dict[dictpos]) << SHIFTBITS) ^ dict[dictpos + 1];

		k = dictpos + bytestodo - compressFloor;  /* calculate end of sector */

		for (i = dictpos; i < k; i++)
		{
			nextlink[i] = hash[j = (((j << SHIFTBITS) & (HASHSIZE - 1)) ^ dict[i + 2])];
			hash[j] = i;
		}
	}
}
/* finds match for string at position dictpos */
/* this search code finds the longest AND closest
		 match for the string at dictpos */
void LZ77::FindMatch(unsigned int dictpos, unsigned int startlen)
{
	register unsigned int i, j, k;
	unsigned char l;

	i = dictpos; matchlength = startlen; k = comparesCeil;
	l = dict[dictpos + matchlength];

	do
	{
		if ((i = nextlink[i]) == NIL) return;   /* get next string in list */

		if (dict[i + matchlength] == l)        /* possible larger match? */
		{
			for (j = 0; j < MAXMATCH; j++)          /* compare strings */
				if (dict[dictpos + j] != dict[i + j]) break;

			if (j > matchlength)  /* found larger match? */
			{
				matchlength = j;
				matchpos = i;
				if (matchlength == MAXMATCH) return;  /* exit if largest possible match */
				l = dict[dictpos + matchlength];
			}
		}
	} while (--k);  /* keep on trying until we run out of chances */

}
/* finds dictionary matches for characters in current sector */
void LZ77::DictSearch(unsigned int dictpos, unsigned int bytestodo)
{

	register unsigned int i, j;

	i = dictpos; j = bytestodo;

	while (j) /* loop while there are still characters left to be compressed */
	{
		FindMatch(i, compressFloor);

		if (matchlength > j) matchlength = j;     /* clamp matchlength */

		if (matchlength > compressFloor)  /* valid match? */
		{
			SendMatch(matchlength, (i - matchpos) & (DICTSIZE - 1));
			i += matchlength;
			j -= matchlength;
		}
		else
		{
			SendChar(dict[i++]);
			j--;
		}
	}

}
void LZ77::Compress() {
	unsigned int dictpos, deleteflag, sectorlen;
	unsigned long bytescompressed;

	InitEncode();

	dictpos = deleteflag = 0;

	bytescompressed = 0;

	while (1)
	{
		/* delete old data from dictionary */
		if (deleteflag) DeleteData(dictpos);

		/* grab more data to compress */
		if ((sectorlen = LoadDict(dictpos)) == 0) break;

		/* hash the data */
		HashData(dictpos, sectorlen);

		/* find&write dictionary matches */
		DictSearch(dictpos, sectorlen);

		bytescompressed += sectorlen;

		printf("\r%ld", bytescompressed);

		dictpos += SECTORLEN;

		/* wrap back to beginning of dictionary when its full */
		if (dictpos == DICTSIZE)
		{
			dictpos = 0;
			deleteflag = 1;   /* ok to delete now */
		}
	}

	/* Send EOF flag */
	SendMatch(MAXMATCH + 1, 0);

	/* Flush bit buffer */
	if (bitsin) SendBits(0, 8 - bitsin);

	return;
}
void LZ77::Decompress() {
	register unsigned int i, j, k;
	unsigned long bytesdecompressed;
	i = 0;
	bytesdecompressed = 0;
	int64_t countC=0,countM=0,countL=0;
	for (; ; )
	{
		/* character or match? */
		if (ReadBits(1) == 0)	//Character
		{
			countC++;
			dict[i++] = ReadBits(CHARBITS);
			if (i == DICTSIZE)
			{
				if (fwrite(dict, sizeof(char), DICTSIZE, outfile) == EOF)
				{
					printf("\nerror writing to output file");
					exit(EXIT_FAILURE);
				}
				i = 0;
				bytesdecompressed += DICTSIZE;
				printf("\r%ld", bytesdecompressed);
			}
		}
		else					//Match
		{
			/* get match length from input stream */
			k = (compressFloor + 1) + ReadBits(MATCHBITS);
			
			if (k == (MAXMATCH + 1))      /* Check for EOF flag */
			{
				if (fwrite(dict, sizeof(char), i, outfile) == EOF)
				{
					printf("\nerror writing to output file");
					exit(EXIT_FAILURE);
				}
				bytesdecompressed += i;
				printf("\r%ld", bytesdecompressed);
				printf("\nCharLostByte:%d\nBytesOnMatch:%d\nCompressedBytes:%d\nMatchEfficency:%d",countC/8,countM*(DICTBITS+MATCHBITS+1)/8,countL,(countL*(CHARBITS+1))/(countM*(DICTBITS+MATCHBITS+1)));
				printf(".%d\nTotalBytesWin:%d",REM(countL*(CHARBITS+1),countM*(DICTBITS+MATCHBITS+1)),((countL-countM*(DICTBITS+MATCHBITS+1)/8)-countC/8));
				return;
			}
			countM++;
			countL+=k;
			/* get match position from input stream */
			j = ((i - ReadBits(DICTBITS)) & (DICTSIZE - 1));
			
			do
			{
				dict[i++] = dict[j++];
				j &= (DICTSIZE - 1);//Warp to begining, if reach end	(j + k) >= DICTSIZE
				if (i == DICTSIZE)//Check for end of dictionary 	(i + k) >= DICTSIZE
				{
					if (fwrite(dict, sizeof(char), DICTSIZE, outfile) == EOF)
					{
						printf("\nerror writing to output file");
						exit(EXIT_FAILURE);
					}
					i = 0;
					bytesdecompressed += DICTSIZE;
					printf("\r%ld", bytesdecompressed);
				}
			} while (--k);
		}
	}
	
}
int main(int argc, char *argv[]){
	LZ77::IStruct tstr;
	LZ77 LZ77Archiver(tstr);
	char *s;

	if (argc != 4)
	{
		printf("\n'prog1 e file1 file2' encodes file1 into file2.\n"
			"'prog1 d file2 file1' decodes file2 into file1.\n");
		return EXIT_FAILURE;
	}
	if ((s = argv[1], s[1] || strpbrk(s, "DEde") == NULL)
		|| (s = argv[2], (LZ77Archiver.infile = fopen(s, "rb")) == NULL)
		|| (s = argv[3], (LZ77Archiver.outfile = fopen(s, "wb")) == NULL)) {
		printf("??? %s\n", s);  return EXIT_FAILURE;
	}

	/* allocate 4k I/O buffers for a little speed */
	setvbuf(LZ77Archiver.infile, NULL, _IOFBF, 4096);
	setvbuf(LZ77Archiver.outfile, NULL, _IOFBF, 4096);

	if (toupper(*argv[1]) == 'E')
	{
		printf("Compressing %s to %s\n", argv[2], argv[3]);
		LZ77Archiver.Compress();
		
	}
	else
	{
		printf("Decompressing %s to %s\n", argv[2], argv[3]);
		LZ77Archiver.Decompress();
	}

	fclose(LZ77Archiver.infile);  fclose(LZ77Archiver.outfile);
	return EXIT_SUCCESS;
}