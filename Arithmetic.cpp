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

void Arithmetic::GenerateTable(std::string* name) {
    
    std::ifstream iFile(name->c_str(), std::ios_base::in);
    unsigned long nBytes = 0;
    char symb;
    while(iFile.get(symb)) {
        
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
            TTable::iterator prev = iter;
            
            --prev;
            ++(*iter)->second;
            
            while((*prev)->second < (*iter)->second) {
                
                std::pair<const char, long double>* tmp = *prev;
                *prev = *iter;
                *iter = tmp;
                
                if(prev == probability.begin()) {
                    break;
                }
                else {
                    --prev;
                }
                --iter;
            }
        }
        
        ++nBytes;
    }
    
    iFile.close();
    
    for(int i = 0; i < probability.size(); ++i) {
        probability[i]->second = probability[i]->second / nBytes;
    }
    
    return;
}

void Arithmetic::GetTable() {
    char symb = 0;
    long double prob = 0;
    input.get(alphabet);
    unsigned long tmp = alphabet;
    while(!input.eof() && alphabet) {
        input.get(symb);
        input >> prob;
        std::pair<const char, long double>* tmp = new std::pair<const char, long double>(symb, prob);
        probability.push_back(tmp);
        --alphabet;
    }
    alphabet = tmp;
    return;
}
