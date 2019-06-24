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
        
        /* Имя: OutBinary(std::string name)
         * Что делает: задаёт начальные значения и открывает файл.
         */
        OutBinary(std::string name);
        
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
        bool        operator << (OutBinary& file, size_t const &bit);
    private:
        
        ofstream    out;
        
        bool        state;
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
        
        /* Имя: InBinary(std::string name)
         * Что делает: задаёт начальные значения и открывает файл.
         */
        InBinary(std::string name);
        
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
        
        /* Имя: Read(size_t size);
         * Что делает: считывает из файла size байт. 
         * В случае успеха - возвращает указатель на считанные данные. 
         * Они будут находиться в куче, так что надо не забыть освободить от них память.
         * В случае неудачи - возвращает nullptr.
         */
        char*       Read(size_t size);
        
        /* Имя: ReadBin(size_t* n);
         * Что делает: считывает из файла бит. Этот бит заносит в n и возвращает указатель на n. 
         * В случае ошибки - возвращает nullptr.
         */
        char*       ReadBin(size_t* n);
        
        /* Что делает: считывает из файла бит и заносит в переменную. 
         * В случае успеха - true. Иначе - false.
         */
        bool operator >> (InBinary& file, size_t const &bit);
    private:
        
        ifstream    in;
        
        bool        state;
        char        head;
        char        block;
};

#endif /* BFILE_H */
