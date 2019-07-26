#ifndef BFILE_H
#define BFILE_H

#include <iostream>
#include <fstream>
#include <string>

/* Запись в файл.
 */
class OutBinary {
    public:
        
        /* Что делает: задаёт начальные значения. Файл не будет открыт.
         */
        OutBinary();
        
        /* Что делает: открывает файл. 
         * В случае успеха - true. Иначе - false.
         */
        bool        Open(std::string* name);
        
        /* Что делает: закрывает файл. 
         * В случае успеха - true. Иначе - false.
         */
        bool        Close();
        
        /* Что делает: записывает файл объект obj размером size байт.
         * В случае успеха - true. Иначе - false.
         */
        bool        Write(const char* obj, size_t size);
        
        /* Что делает: записывает файл нужный бит. 
         * В случае успеха - true. Иначе - false.
         */
        bool        WriteBin(size_t bit);
        
        /* Что делает: возврашает размер файла в виде числа байт.
         */
        unsigned long long SizeFile();
        
        /* Что делает: записывает файл нужный бит. 
         * В случае успеха - true. Иначе - false.
         */
        friend bool operator << (OutBinary& file, size_t const &bit);
    private:
        
        std::ofstream    out;
        
        std::string      name;
        unsigned char    head;
        unsigned char    block;
};



/* Считывание из файла.
 */
class InBinary {
    public:
        
        /* Что делает: задаёт начальные значения. Файл не будет открыт.
         */
        InBinary();
        
        /* Что делает: открывает файл. 
         * В случае успеха - true. 
         * Иначе - false.
         */
        bool        Open(std::string* name);
        
        /* Что делает: закрывает файл. 
         * В случае успеха - true. 
         * Иначе - false.
         */
        bool        Close();
        
        /* Что делает: считывает из файла size байт. 
         * В случае успеха - возвращает true.
         * В случае неудачи - возвращает false.
         */
        bool        Read(char* obj, size_t size);
        
        /* Что делает: считывает из файла бит. Этот бит заносит в bit и возвращает true. 
         * В случае ошибки - возвращает false.
         */
        bool        ReadBin(char* bit);
        
        /* Что делает: возврашает размер файла в виде числа байт.
         */
        unsigned long long SizeFile();
        
        /* Что делает: считывает из файла бит и заносит в переменную.
         * В случае успеха - true. 
         * Иначе - false.
         */
        friend bool operator >> (InBinary& iFile, char &bit);
    private:
        
        std::ifstream    in;
        
        std::string      name;
        unsigned char    head;
        unsigned char    block;
};

#endif /* BFILE_H */
