#include "pch.h"
#include "LZ77.h"


LZ77::LZ77() : bitsin(0)
{
}

/* writes multiple bit codes to the output stream */
void LZ77::SendBits(unsigned int bits, unsigned int numbits)
{

	bitbuf |= (bits << bitsin);

	bitsin += numbits;

	if (bitsin > 16)         /* special case when # bits in buffer exceeds 16 *///make with uint64 and remove this
	{
		if (fputc(bitbuf & 0xFF, outfile) == EOF)
		{
			printf("\nerror writing to output file");
			exit(EXIT_FAILURE);
		}
		bitbuf = bits >> (8 - (bitsin - numbits));
		bitsin -= 8;
		counter++;
	}

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

	register unsigned int i;

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

	SendBits(matchlen - (THRESHOLD + 1), MATCHBITS);

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

	if (bytestodo <= THRESHOLD)   /* not enough bytes in sector for match? */
		for (i = 0; i < bytestodo; i++) nextlink[dictpos + i] = NIL;
	else
	{
		/* matches can't cross sector boundries */
		for (i = bytestodo - THRESHOLD; i < bytestodo; i++)
			nextlink[dictpos + i] = NIL;

		j = (((unsigned int)dict[dictpos]) << SHIFTBITS) ^ dict[dictpos + 1];

		k = dictpos + bytestodo - THRESHOLD;  /* calculate end of sector */

		for (i = dictpos; i < k; i++)
		{
			nextlink[i] = hash[j = (((j << SHIFTBITS) & (HASHSIZE - 1)) ^ dict[i + THRESHOLD])];
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

	i = dictpos; matchlength = startlen; k = MAXCOMPARES;
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

#if (GREEDY == 0)

	unsigned int matchlen1, matchpos1;

	/* non-greedy search loop (slow) */

	i = dictpos; j = bytestodo;

	while (j) /* loop while there are still characters left to be compressed */
	{
		FindMatch(i, THRESHOLD);

		if (matchlength > THRESHOLD)
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
					SendChar(dict[i++]);
					j--;
				}
				else
				{
					if (matchlen1 > j)
					{
						matchlen1 = j;
						if (matchlen1 <= THRESHOLD) { SendChar(dict[i++]); j--; break; }
					}

					SendMatch(matchlen1, (i - matchpos1) & (DICTSIZE - 1));
					i += matchlen1;
					j -= matchlen1;
					break;
				}
			}
		}
		else
		{
			SendChar(dict[i++]);
			j--;
		}
	}

#else

	/* greedy search loop (fast) */

	i = dictpos; j = bytestodo;

	while (j) /* loop while there are still characters left to be compressed */
	{
		FindMatch(i, THRESHOLD);

		if (matchlength > j) matchlength = j;     /* clamp matchlength */

		if (matchlength > THRESHOLD)  /* valid match? */
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

#endif

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

		/* find dictionary matches */
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

void LZ77::DictSearch2(unsigned int dictpos, unsigned int bytestodo, std::string text)
{

	register unsigned int i, j;

#if (GREEDY == 0)

	size_t matchlen1, matchpos1;

	/* non-greedy search loop (slow) */

	i = dictpos; j = bytestodo;
	size_t left_buff = 0;
	size_t right_buff = left_buff + 4 * KB;
	while (j) /* loop while there are still characters left to be compressed */
	{
		if (right_buff < i) {
			left_buff += i - right_buff ;
			right_buff = i;
		}
		matchpos = text.find(&text[i], left_buff, THRESHOLD + 1);
		if (matchpos != i)
		{
			matchlength = THRESHOLD + 1;
			for (;;) {

				while (matchlength < MAXMATCH && ((matchpos1 = text.find(&text[i], matchpos, matchlength + 1)) < i)) {//&&i+matchlength<right
					matchpos = matchpos1;
					++matchlength;
				}

				if (matchlength < MAXMATCH && ((matchpos1 = text.find(&text[i + 1], left_buff, matchlength + 1)) < i + 1)) {
					SendChar(text[i++]);
					matchpos = matchpos1;
					++matchlength;
					j--;
					if (right_buff < i) {
						left_buff += i - right_buff;
						right_buff = i;
					}
					continue;
				}

				while ((matchpos1 = text.find(&text[i], matchpos+1, matchlength)) < i) {//shall be removed?
					matchpos = matchpos1;
				}

				break;
			}

			if (matchlength > j) {
				matchlength = j;
				if (matchlength <= THRESHOLD) {
					SendChar(text[i++]);
					j--;
					continue;
				}
			}

			SendMatch(matchlength, (i - matchpos) & (DICTSIZE - 1));
			i += matchlength;
			j -= matchlength;
		}
		else
		{
			SendChar(text[i++]);
			j--;
		}
	}

			/*for (; ; )
			{
				//FindMatch(i + 1, matchlen1);

				if (matchlength > matchlen1)
				{
					matchlen1 = matchlength;
					matchpos1 = matchpos;
					SendChar(dict[i++]);
					j--;
				}
				else
				{
					if (matchlen1 > j)
					{
						matchlen1 = j;
						if (matchlen1 <= THRESHOLD) { SendChar(dict[i++]); j--; break; }
					}

					SendMatch(matchlen1, (i - matchpos1) & (DICTSIZE - 1));
					i += matchlen1;
					j -= matchlen1;
					break;
				}
			}*/
		

#else

	/* greedy search loop (fast) */

	i = dictpos; j = bytestodo;

	while (j) /* loop while there are still characters left to be compressed */
	{
		FindMatch(i, THRESHOLD);

		if (matchlength > j) matchlength = j;     /* clamp matchlength */

		if (matchlength > THRESHOLD)  /* valid match? */
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

#endif
	std::cout << text[i];
}

void LZ77::Compress2() {
	unsigned int dictpos, deleteflag, sectorlen;
	unsigned long bytescompressed;
	dictpos = deleteflag = 0;

	bytescompressed = 0;

	//InitEncode();

	
	std::string text;
	std::ifstream file("test.txt", std::ios::in | std::ios::ate);
	std::ios_base::sync_with_stdio(false);
	if (file) {
		std::ifstream::streampos filesize = file.tellg();
		text.reserve(32 * MB); //filesize);//put text in string part by part

		file.seekg(0);
		//text.resize(file.read(&text[0], 32*MB));

		while (filesize) {
			while (1)
			{

				/* find dictionary matches */
				DictSearch2(dictpos, text.size(), text);
				//if (dictpos > 0) break;


				bytescompressed += text.size();

				printf("\r%ld", bytescompressed);

				dictpos += SECTORLEN;

				/* wrap back to beginning of dictionary when its full */
				if (dictpos == DICTSIZE)
				{
					dictpos = 0;
					deleteflag = 1;   /* ok to delete now */
				}
				break;
			}DictSearch2(dictpos, text.size(), text);
		}

	}

	

	dictpos = deleteflag = 0;

	bytescompressed = 0;

	while (1)
	{

		/* find dictionary matches */
		DictSearch2(dictpos, text.size(),text);
		//if (dictpos > 0) break;


		bytescompressed += text.size();

		printf("\r%ld", bytescompressed);

		dictpos += SECTORLEN;

		/* wrap back to beginning of dictionary when its full */
		if (dictpos == DICTSIZE)
		{
			dictpos = 0;
			deleteflag = 1;   /* ok to delete now */
		}
		break;
	}

	/* Send EOF flag */
	SendMatch(MAXMATCH + 1, 0);

	/* Flush bit buffer */
	if (bitsin) SendBits(0, 8 - bitsin);

	return;
}

void LZ77::Decompress() {

}

LZ77::~LZ77()
{
}

void LZ77::Compress3() {
	unsigned int dictpos, sectorlen;
	unsigned long bytescompressed;
	bool deleteflag;

//	InitSearchStruct();

	dictpos = bytescompressed = 0;
	deleteflag = false;	
	
	while (1)
	{
		/* delete old data from dictionary */
		if (deleteflag) DeleteData(dictpos);

		/* grab more data to compress */
		if ((sectorlen = LoadDict(dictpos)) == 0) break;

		/* hash the data */
		HashData(dictpos, sectorlen);

		/* find dictionary matches */
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