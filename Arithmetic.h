#pragma once
#include "Compressor.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

typedef std::vector<std::pair<const char, long double>*> TTable;

class Arithmetic: public Compressor {    
public:
	
                                        /* Что делает: Сжимает данные
                                         */
	void                                Compress(std::string) override;

                                        /* Что делает: Расжимает данные
                                         */
	void                                Decompress(std::string) override;


                                        Arithmetic();
	virtual                             ~Arithmetic();

private:
    
    void                                GenerateTable(std::string*);
    void                                GetTable(std::string*);
    
    std::ifstream                       input;
    std::ofstream                       output;
    
    uint_least32_t                      alphabet;
    
    TTable                              probability;
};

/* 
 * [] - общее для всех алгоритмов сжатия;
 * () - специфичное для алгоритмов сжатия;
 * 
 * Структура архива:
 * 
 * [] {char}                      - Вид сжатия;
 * [] {unsigned long long}        - Размер архива до сжатия;
 * 
 * () {unsgined long}             - M символов в таблице вероятностей;
 * () {(char + long double) * M}  - Сама таблица вероятностей (символ : вероятность);
 * 
 * [] N байт                      - Содержимое архива;
 * 
 */
