#include <iostream>
#include <random>

//генератор тестов
//файл указывайте с помощью >>

int main() {
	std::random_device rd;
	std::mt19937 mersenne(rd());
	unsigned long long int count;
	std::cin >> count;
	int ran;
	for (unsigned long long int i = 0; i < count; i++) {
		ran = mersenne() % 128;
		std::cout << (char) ((ran > 32 && ran != 127) ? ran : ((ran + 40)) % 127);
	}
	std::cout << std::endl;
	return 0;
}
