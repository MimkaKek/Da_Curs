#include <iostream>
#include <random>
#include <sstream>

//генератор тестов
//файл указывайте с помощью >>

int main(int argc, char *argv[]) {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	unsigned long long int count;
	if (argc == 0) {
		std::cin >> count;
	}
	else {
		std::stringstream tmp(argv[1]);
		tmp >> count;
	}
	count--;
	int ran;
	/* возможно я переборщил с рандомизатором
	for (unsigned long long int i = 0; i < count; i++) {
		ran = mersenne() % 128;
		std::cout << (char) ((ran > 32 && ran != 127) ? ran : ((ran + 40)) % 127);
	}*/
	for (unsigned long long int i = 0; i < count; i++) {//LZWTEST
		ran = mersenne() % 3;
		std::cout << (ran == 0 ? 'q' : (ran == 1 ? 'w' : 'e'));
	}
	//std::cout << std::endl;
	return 0;
}
