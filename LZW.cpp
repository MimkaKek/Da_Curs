#include "LZW.h"

TLZW::TLZW(int compressionRatio) {
	unsigned long long int fileSize = GetSize(); // TODO SLAVA
	if (compressionRatio == DECOMPRESS) {
		this->compressionTree = new TPrefix('d', 0);
		return;
	}
	if (fileSize < LOW_BORDER && fileSize > 3) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= 3;
				break;
			case NORMAL:
				fileSize *= (double) 2 / 3;
				break;
		}
	}
	else if (fileSize >= LOW_BORDER && fileSize < MEDIUM_BORDER) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= 100;
				break;
			case NORMAL:
				fileSize /= 10;
				break;
		}
	}
	else if (fileSize >= MEDIUM_BORDER && fileSize < HIGH_BORDER) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= 1000000;
				break;
			case NORMAL:
				fileSize /= 1000;
				break;
		}
	}
	else {
		switch (compressionRatio) {
			case FAST:
				fileSize = HIGH_BORDER / 1000000000000LLU;
				break;
			case NORMAL:
				fileSize = HIGH_BORDER / 1000000;
				break;
			case HIGH:
				fileSize = HIGH_BORDER;
				break;
		}
	}
	this->compressionTree = new TPrefix('c', fileSize);
}
