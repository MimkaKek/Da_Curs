#include "BFile.h"

OutBinary::OutBinary() {
    head = 0;
    head |= 1 << 7;
    block = 0;
}

bool OutBinary::Open(std::string name) {
    if(out.is_open()) {
        return false;
    }
    else {
        out.open(name.c_str(), std::ofstream::out);
        if(!out) {
            return false;
        }
        else {
            head = 0;
            head |= 1 << 7;
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

bool OutBinary::Write(char* obj, size_t size) {
    if(out.is_open()) {
        
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
            head |= 1 << 7;
        }
        return true;
    }
    else {
        return false;
    }
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
            file.head |= 1 << 7;
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

bool InBinary::Open(std::string name) {
    if(in.is_open()) {
        return false;
    }
    else {
        in.open(name.c_str(), std::ofstream::in);
        if(!in) {
            return false;
        }
        else {
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

bool InBinary::ReadBin(char* n) {
    if(!in.eof()) {
        if(!head) {
            in >> block;
            head |= 1 << 7;
        }
        ((block & head) != 0) ? (*n = 1) : (*n = 0);
        head >>= 1;
        return true;
    }
    else {
        return false;
    }
}

bool operator >> (InBinary& file, size_t &bit) {
    if(!file.in.eof()) {
        if(!file.head) {
            file.in >> file.block;
            file.head |= 1 << 7;
        }
        ((file.block & file.head) != 0) ? (bit = 1) : (bit = 0);
        file.head >>= 1;
        return true;
    }
    else {
        return false;
    }
}


/* BFile.cpp */
