#pragma once
#include "Compressor.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>

class Arithmetic: public Compressor {    
public:
	
                                        /* 
                                         * Что делает: Сжимает данные
                                         */
	void                                Compress(std::string) override;

                                        /* 
                                         * Что делает: Расжимает данные
                                         */
	void                                Decompress(std::string) override;


                                        Arithmetic();
	virtual                             ~Arithmetic();

private:
    
    void                                GenerateTable(std::string*);
    void                                GetTable(std::string*);
    
    std::ifstream                       input;
    std::ofstream                       output;
    unsigned long                       nSymbs;
    
    //TODO Или всё таки лучше std::vector?
    std::map<const char, long double>   probability;
};
