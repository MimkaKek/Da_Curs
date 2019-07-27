#ifndef TPREFIX_H
#define TPREFIX_H

#include "Globals.h"
#include "BFile.h"

// класс для префиксного дерева в LZW
class TPrefix {
public:

	/* Конструктор корня */
	TPrefix(unsigned long long int, InBinary*, OutBinary*);

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
	std::vector<std::pair<unsigned char, TPrefix*>> Next;

	/* номер слова в таблице */
	unsigned long long int NumberOfWord;

	/* последняя буква из файла */
	static unsigned char LastLetter;

	/* номер последнего добавленного слова */
	static unsigned long long int LastNumber;

	/* максимальная граница кол-ва слов перед созданием нового дерева */
	static const unsigned long long int Border;
	
	static InBinary* Read; 

	static OutBinary* Write;
};

#endif
