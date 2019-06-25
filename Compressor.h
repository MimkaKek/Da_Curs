#pragma once
class Compressor
{
public:
	Compressor();
	virtual void Compress()=0;
	virtual void Decompress()=0;
	virtual ~Compressor();
};

