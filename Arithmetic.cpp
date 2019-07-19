#include "Arithmetic.h"

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
    //TODO переписать чтения файла, чтобы избегать игнора пробелов и прочих символов
    std::ifstream iFile(name->c_str(), std::ios_base::in);
    unsigned long count = 0;
    char symb;
    while(!iFile.eof()) {
        iFile.symb;
        std::map<const char, long double>::iterator iter = probability.find(symb);
        if(iter == probability.end()) {
            probability.insert(std::pair<const char, long double> (symb, 1));
            ++nSymbs;
        }
        else {
            ++iter->second;
        }
        ++count;
    }
    
    iFile.close();
    
    for(std::map<const char, long double>::iterator iter = probability.begin(); iter != probability.end(); ++iter) {
        iter->second = iter->second / count;
    }
    
    return;
}

void Arithmetic::GetTable(std::string* name) {
    //TODO переписать чтения файла, чтобы избегать игнора пробелов и прочих символов
    std::ifstream iFile(name->c_str(), std::ios_base::in);
    char nSymbs;
    char symb;
    iFile >> nSymbs;
    while(!iFile.eof() && nSymbs) {
        iFile >> symb;
        std::map<const char, long double>::iterator iter = probability.find(symb);
        if(iter == probability.end()) {
            probability.insert(std::pair<const char, long double> (symb, 1));
        }
        else {
            ++iter->second;
        }
    }
}
