#ifndef TPREFIX_H
#define TPREFIX_H

#include "Globals.h"
#include "BFile.h"

// класс для префиксного дерева в LZW
class TPrefix {
public:

	/* Конструктор корня */
	TPrefix(unsigned long long int);

	/* Конструктор для прочих узлов */
	TPrefix()

	/* Добавление новых узлов в дерево */
	bool Update(char);

	/* особый апдейт для корня */
	int UpdateForRoot();

	/* очистка дерева после переполнения */
	void Clear(bool);

	/* Деструктор */
	~TPrefix();

private:

	/* вектор потомков и путей в них */
	std::vector<std::pair<unsigned char, TPrefix*>> next;

	/* номер слова в таблице */
	unsigned long long int numberOfWord;

	/* последняя буква из файла */
	static unsigned char lastLetter;

	/* номер последнего добавленного слова */
	static unsigned long long int lastNumber;

	/* максимальная граница кол-ва слов перед созданием нового дерева */
	static const unsigned long long int border;
	
	static FILE read; 

	static FILE write;
};

#endif
