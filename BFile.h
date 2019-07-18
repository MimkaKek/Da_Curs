#ifndef BFILE_H
#define BFILE_H

#include <fstream>
#include <string>

/* Запись в файл.
 */
class OutBinary {
    public:
        
        /* Имя: OutBinary()
         * Что делает: задаёт начальные значения. Файл не будет открыт.
         */
        OutBinary();
        
        /* Имя: Open(std::string name)
         * Что делает: открывает файл. 
         * В случае успеха - true. Иначе - false.
         */
        bool        Open(std::string name);
        
        /* Имя: Close();
         * Что делает: закрывает файл. 
         * В случае успеха - true. Иначе - false.
         */
        bool        Close();
        
        /* Имя: Write(void* obj, size_t size);
         * Что делает: записывает файл объект obj размером size байт.
         * В случае успеха - true. Иначе - false.
         */
        bool        Write(char* obj, size_t size);
        
        /* Имя: WriteBin(size_t bit);
         * Что делает: записывает файл нужный бит. 
         * В случае успеха - true. Иначе - false.
         */
        bool        WriteBin(size_t bit);
        
        /* Что делает: записывает файл нужный бит. 
         * В случае успеха - true. Иначе - false.
         */
        friend bool operator << (OutBinary& file, size_t const &bit);
    private:
        
        std::ofstream    out;
        
        char        head;
        char        block;
};



/* Считывание из файла.
 */
class InBinary {
    public:
        
        /* Имя: InBinary()
         * Что делает: задаёт начальные значения. Файл не будет открыт.
         */
        InBinary();
        
        /* Имя: Open(std::string name)
         * Что делает: открывает файл. 
         * В случае успеха - true. Иначе - false.
         */
        bool        Open(std::string name);
        
        /* Имя: Close();
         * Что делает: закрывает файл. 
         * В случае успеха - true. Иначе - false.
         */
        bool        Close();
        
        /* Имя: Read(char* obj, size_t size);
         * Что делает: считывает из файла size байт. 
         * В случае успеха - возвращает true.
         * В случае неудачи - возвращает false.
         */
        bool        Read(char* obj, size_t size);
        
        /* Имя: ReadBin(char* n);
         * Что делает: считывает из файла бит. Этот бит заносит в n и возвращает true. 
         * В случае ошибки - возвращает false.
         */
        bool        ReadBin(char* n);
        
        /* Что делает: считывает из файла бит и заносит в переменную.
         * В случае успеха - true. Иначе - false.
         */
        friend bool operator >> (InBinary& file, size_t &bit);
    private:
        
        std::ifstream    in;
        
        char        head;
        char        block;
};

#endif /* BFILE_H */
