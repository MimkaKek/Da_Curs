#include "Arithmetic.h"

Arithmetic::Arithmetic() {
    alphabet = 0;
}

Arithmetic::~Arithmetic() {
    for(int i = 0; i < probability.size(); ++i) {
        delete probability[i];
    }
}

void Arithmetic::Compress(std::string name) {
    //TODO алгоритм сжатия
    std::cout << "1";
    return;
}

void Arithmetic::Decompress(std::string name) {
    //TODO алгоритм разжатия
    std::cout << "2";
    return;
}

//TODO Сделать преобразование имени файла с .txt на .gz

void Arithmetic::GenerateTable(std::string* name) {
    
    unsigned long           nBytes = 0;
    char                    symb   = 0;
    
    if(S_GBT || S_GPT || S_GI) {
        std::cout << "|======================|" << std::endl;
        std::cout << "|      GENERATION      |" << std::endl;
        std::cout << "|======================|" << std::endl;   
    }
    
    input.open(name->c_str(), std::ios_base::in);
    while(input.get(symb)) {
        
        TTable::iterator iter = probability.begin();
        
        while(iter != probability.end() && (*iter)->first != symb) {
            ++iter;
        }
        
        if(iter == probability.end()) {
            std::pair<const char, long double>* tmp = new std::pair<const char, long double>(symb, 1);
            probability.push_back(tmp);
            ++alphabet;
        }
        else {

            ++(*iter)->second;

            if(iter != probability.begin()) {
                TTable::iterator prev = iter;
                --prev;
                
                while((*iter)->second > (*prev)->second) {
                    std::pair<const char, long double>* tmp = *prev;
                    *prev = *iter;
                    *iter = tmp;
                    if(prev != probability.begin()) {
                        --prev; 
                    }
                    else {
                        break;
                    }
                    --iter;
                }
            }
        }
        
        ++nBytes;
    }
    input.close();

    cBorders = new std::pair<char, long double>[alphabet + 1];

    cBorders[0].first  = probability[0]->first;
    cBorders[0].second = 0;
    
    if(S_GI) {
        std::cout << "alphabet is equal " << alphabet << std::endl;
        std::cout << nBytes << " bytes was readed from \"" << *name << "\"" << std::endl;   
    }
    
    *name = "File.bin";
    output.open(name->c_str(), std::ios_base::out | std::ios_base::binary);
    
    output.write( (char*) &alphabet          , sizeof (uint_least32_t));
    output.write( (char*) &nBytes            , sizeof (unsigned long) );
    output.write( (char*) &cBorders[0].first , sizeof (char)          );
    output.write( (char*) &cBorders[0].second, sizeof (long double)   );

    if(S_GPT) {
        std::cout << "|======================|" << std::endl;
        std::cout << "| Table of probability |" << std::endl;
        std::cout << "|======================|" << std::endl;
        
    }
    
    for(int i = 0; i < probability.size(); ++i) {

        probability[i]->second = (double) probability[i]->second / nBytes;

        if(S_GPT) {
            std::cout << "[" << probability[i]->first << "] : [" << probability[i]->second << "]" << std::endl;
        }
    }

    for(int i = 0; i < probability.size() - 1; ++i) {
        
        cBorders[i + 1].first  = probability[i + 1]->first;
        cBorders[i + 1].second = cBorders[i].second + probability[i]->second;

        output.write( (char*) &cBorders[i + 1].first, sizeof(char));
        output.write( (char*) &cBorders[i + 1].second, sizeof(long double));
        
    }
    if(S_GBT) {
        std::cout << "|======================|" << std::endl;
        std::cout << "|   Table of borders   |" << std::endl;
        std::cout << "|======================|" << std::endl;
    
        for(int i = 0; i < probability.size(); ++i) {
            std::cout << "[" << cBorders[i].first << "] : [" << cBorders[i].second << "]" << std::endl;
        }
    }    
    output.close();

    return;
}

void Arithmetic::LoadTable() {

    unsigned long           nBytes = 0;
    uint_least32_t          step   = 0;
    char                    symbol = 0;
    long double             prob   = 0;

    if(S_LBT || S_LPT || S_LI) {
        std::cout << "|======================|" << std::endl;
        std::cout << "|      LOAD TABLE      |" << std::endl;
        std::cout << "|======================|" << std::endl;
    }
    
    input.open(name->c_str(), std::ios_base::in | std::ios_base::binary);
    
    input.read( (char*) &alphabet, sizeof(uint_least32_t));
    input.read( (char*) &nBytes  , sizeof(unsigned long) );

    if(S_LI) {
        std::cout << "alphabet is equal " << alphabet << std::endl;
        std::cout << nBytes << " bytes was readed from \"" << *name << "\"" << std::endl;
    }
    
    cBorders = new std::pair<char, long double>[alphabet];

    if(S_LPT) {
        std::cout << "|======================|" << std::endl;
        std::cout << "| Table of probability |" << std::endl;
        std::cout << "|======================|" << std::endl;
    }
    
    while(!input.eof() && alphabet) {

        input.read( (char*) &symbol, sizeof(char)       );
        input.read( (char*) &prob  , sizeof(long double));

        std::pair<const char, long double>* tmp = new std::pair<const char, long double>(symbol, prob);
        probability.push_back(tmp);
        
        if(S_LPT) {
            std::cout << "[" << probability[step]->first << "] : [" << probability[step]->second << "]" << std::endl;
        }
        
        --alphabet;
        ++step;
    }
    alphabet = step;
    
    cBorders[0].first  = probability[0]->first;
    cBorders[0].second = 0;
    
    if(S_LBT) {
        std::cout << "|======================|" << std::endl;
        std::cout << "|   Table of borders   |" << std::endl;
        std::cout << "|======================|" << std::endl;
        std::cout << "[" << cBorders[0].first << "] : [" << cBorders[0].second << "]" << std::endl;
    }
    for(int i = 0; i < alphabet - 1; ++i) {
        
        cBorders[i + 1].first  = probability[i + 1]->first;
        cBorders[i + 1].second = cBorders[i].second + probability[i]->second;

        if(S_LBT) {
            std::cout << "[" << cBorders[i + 1].first << "] : [" << cBorders[i + 1].second << "]" << std::endl;
        }
    }

    return;
}
