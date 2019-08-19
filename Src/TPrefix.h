#ifndef TPREFIX_H
#define TPREFIX_H

#include "Globals.h"
#include "BFile.h"
#include <limits>

// класс для префиксного дерева в LZW
class TPrefix {
public:
	/* Конструктор корня */
	TPrefix(unsigned long long int, InBinary*, OutBinary*);

	/* Конструктор для прочих узлов */
	TPrefix();

	/* Добавление новых узлов в дерево */
	int Update(char);

	/* особый апдейт для корня */
	int UpdateForRoot();

	/* очистка дерева после переполнения */
	void Clear(bool);

	/* Деструктор */
	~TPrefix();

private:

	/* вектор потомков и путей в них */
	std::vector<std::pair<char, TPrefix*>> Next;

	/* номер слова в таблице */
	unsigned long long int NumberOfWord;

	/* последняя буква из файла */
	static char LastLetter;

	/* вспомогательная переменная для чтения нужного кол-ва символов */
	static unsigned long long int NeedToRead;

	/* номер последнего добавленного слова */
	static unsigned long long int LastNumber;

	/* максимальная граница кол-ва слов перед созданием нового дерева */
	static unsigned long long int Border;
	
	static InBinary* ForRead; 

	static OutBinary* ForWrite;

	static unsigned short int Bites;
};

#endif
