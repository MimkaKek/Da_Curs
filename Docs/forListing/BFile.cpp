#include "BFile.h"

TOutBinary::TOutBinary() {
    head = 1 << 7;
    block = 0;
}

bool TOutBinary::Open(std::string* name) {
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

bool TOutBinary::Close() {
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

bool TOutBinary::Write(const char* obj, size_t size) {
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

bool TOutBinary::WriteBin(size_t bit) {
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

unsigned long long TOutBinary::SizeFile() {
    std::ifstream in(name, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

bool operator << (TOutBinary& file, size_t const &bit) {
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

TInBinary::TInBinary() {
    head = 0;
    block = 0;
}

bool TInBinary::Open(std::string* name) {
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

bool TInBinary::Close() {
    in.close();
    if(in.fail()) {
        return false;
    }
    return true;
}

bool TInBinary::Read(char* obj, size_t size) {
    if(!in.eof()) {
        in.read(obj, size);
        return true;
    }
    else {
        return false;
    }
}

bool TInBinary::ReadBin(char* bit) {
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

unsigned long long TInBinary::SizeFile() {
    std::ifstream in(name, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

bool operator >> (TInBinary& iFile, char& bit) {
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
