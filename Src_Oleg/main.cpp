#include "interface.h"

std::vector<bool> keys;

int main(int argc, char *argv[]) {
	clock_t t0 = clock();
	std::map<std::string, int> files;
	keys = {false, false, false, false, false, false, false, false, false};
//			c	   d	  k		 l		r	   t	  1		 9		a
	for (int i = 1; i < argc; ++i) {
		std::string str;
		std::stringstream tmp(argv[i]);
		tmp >> str;
		if (!Parser(&files, str)) {
			return 1;
		}
	}
	if (files.empty()) {
		std::cout << "Compressed data not written to a terminal.\n";
		return 0;
	}
	FileIterator(files);
	clock_t t1 = clock();
	if (keys[8]) {
		std::cout << "\n" << ((double) t1 - t0) / CLOCKS_PER_SEC << " секунд\n";
	}
	return 0;
}
