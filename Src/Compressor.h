#pragma once

class ICompressor {
public:
	ICompressor();
	virtual bool Compress()=0;
	virtual bool Decompress()=0;
	virtual ~ICompressor();
};

