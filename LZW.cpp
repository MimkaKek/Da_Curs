#include "LZW.h"

TLZW::TLZW(int compressionRatio) {
	unsigned long long int fileSize = GetSize(); // TODO SLAVA
	if (compressionRatio == DECOMPRESS) {
		fileSize = 0;
	}
	else if (fileSize < LOW_BORDER && fileSize > 3) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= 3;
				break;
			case NORMAL:
				fileSize *= (2.0 / 3.0);
				break;
		}
	}
	else if (fileSize >= LOW_BORDER && fileSize < MEDIUM_BORDER) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= LOW_FAST;
				break;
			case NORMAL:
				fileSize /= LOW_MEDIUM;
				break;
		}
	}
	else if (fileSize >= MEDIUM_BORDER && fileSize < HIGH_BORDER) {
		switch (compressionRatio) {
			case FAST:
				fileSize /= MEDIUM_FAST;
				break;
			case NORMAL:
				fileSize /= MEDIUM_MEDIUM;
				break;
		}
	}
	else if (fileSize >= HIGH_BORDER) {
		switch (compressionRatio) {
			case FAST:
				fileSize = HIGH_BORDER / HIGH_FAST;
				break;
			case NORMAL:
				fileSize = HIGH_BORDER / HIGH_MEDIUM;
				break;
			case HIGH:
				fileSize = HIGH_BORDER;
				break;
		}
	}
	this->compressionTree = new TPrefix(fileSize);
	return;
}
