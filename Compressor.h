#pragma once

class ICompressor {
public:
	ICompressor();
	virtual void Compress()=0;
	virtual void Decompress()=0;
	virtual ~ICompressor();
};

