#ifndef BFILE_H
#define BFILE_H

#include <iostream>
#include <fstream>
#include <string>

class TOutBinary {
public:
    TOutBinary();
    bool Open(std::string* name);
    bool Close();
    bool Write(const char* obj, size_t size);
    bool WriteBin(size_t bit);
    unsigned long long SizeFile();
    friend bool operator << (TOutBinary& file, size_t const &bit);
private:
    std::ofstream out;
    std::string name;
    unsigned char head;
    unsigned char block;
};

class TInBinary {
public:
    TInBinary();
    bool Open(std::string* name);
    bool Close();
    bool Read(char* obj, size_t size);
    bool ReadBin(char* bit);
    unsigned long long SizeFile();
    friend bool operator >> (TInBinary& iFile, char &bit);
private:
    std::ifstream in;
    std::string name;
    unsigned char head;
    unsigned char block;
};

#endif
