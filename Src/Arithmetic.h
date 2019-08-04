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
    virtual                            ~Arithmetic();

private:
    
                                        /* Что делает: Генерация таблицы вероятностей
                                         *             и отрезков для прямой [0;1]
                                         */
    void                                GenerateTable(std::string*);

                                        /* Что делает: Считывание из файла
                                         *             готовой таблицы вероятностей
                                         *             и создание отрезков для прямой [0;1]
                                         */
    void                                LoadTable(std::string*);

                                        /* что делает: Задаёт новые отрезки на прямой [lower, upper],
                                         *             которые пропорционально расположены
                                         *             по отношению к исходным отрезкам.
                                         */
    void                                SetNewBorders(long double lower, long double upper);
    
    std::ifstream                       input;
    std::ofstream                       output;
    
    unsigned int                        alphabet;

    std::pair<char, long double>*       cBorders;
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
