#include "BFile.h"

OutBinary::OutBinary() {
    head = 1 << 7;
    block = 0;
}

bool OutBinary::Open(std::string* name) {
    if(out.is_open()) {
        return false;
    }
    else {
        out.open(name->c_str(), std::ofstream::out);
        if(!out) {
            return false;
        }
        else {
            this->name = *name;
            head = 1 << 7;
            block = 0;
            return true;
        }
    }
}

bool OutBinary::Close() {
    if(out.is_open()) {
        if(!(head & (1 << 7))) {
            out << block;
        }
        out.close();
        if(out.fail()) {
            return false;
        }
        return true;
    }
    else {
        return false;
    }
}

bool OutBinary::Write(const char* obj, size_t size) {
    if(out.is_open()) {
        if(!(head & (1 << 7))) {
            out << block;
        }
        out.write(obj, size);
        return true;
    }
    else {
        return false;
    }
}

bool OutBinary::WriteBin(size_t bit) {
    
    if(out.is_open()) {
        if(bit) {
            block |= head;
        }
        head >>= 1;
        if(!head) {
            out << block;
            block = 0;
            head = 1 << 7;
        }
        return true;
    }
    else {
        return false;
    }
}

unsigned long long OutBinary::SizeFile() {
    std::ifstream in(name, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

bool operator << (OutBinary& file, size_t const &bit) {
    
    if(file.out.is_open()) {
        if(bit) {
            file.block |= file.head;
        }
        file.head >>= 1;
        if(!file.head) {
            file.out << file.block;
            file.block = 0;
            file.head = 1 << 7;
        }
        return true;
    }
    else {
        return false;
    }
}


InBinary::InBinary() {
    head = 0;
    block = 0;
}

bool InBinary::Open(std::string* name) {
    if(in.is_open()) {
        return false;
    }
    else {
        in.open(name->c_str(), std::ofstream::in);
        if(!in) {
            return false;
        }
        else {
            this->name = *name;
            head = 0;
            block = 0;
            return true;
        }
    }
}

bool InBinary::Close() {
    in.close();
    if(in.fail()) {
        return false;
    }
    return true;
}

bool InBinary::Read(char* obj, size_t size) {
    if(!in.eof()) {
        in.read(obj, size);
        return true;
    }
    else {
        return false;
    }
}

bool InBinary::ReadBin(char* bit) {
    if(!head) {
        if(in >> block) {
            head = 1 << 7;
        }    
        else {
            return false;
        }
    }
    ((block & head) != 0) ? (*bit = 1) : (*bit = 0);
    head >>= 1;
    return true;
}

unsigned long long InBinary::SizeFile() {
    std::ifstream in(name, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

bool operator >> (InBinary& iFile, char& bit) {
    if(!iFile.head) {
        if(iFile.in >> iFile.block) {
            iFile.head = 1 << 7;
        }    
        else {
            return false;
        }
    }
    ((iFile.block & iFile.head) != 0) ? (bit = 1) : (bit = 0);
    iFile.head >>= 1;
    return true;
}


/* BFile.cpp */
