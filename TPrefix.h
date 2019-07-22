#include "Globals.h"

// класс для префиксного дерева в LZW
class TPrefix {
public:

	/* Конструктор корня */
	TPrefix(char, unsigned int);

	/* Конструктор для прочих узлов */
	TPrefix(unsigned int)

	/* особый апдейт для корня */
	void UpdateForRoot();

	/* Добавление новых узлов в дерево */
	void Update(char);

	/* очистка дерева после переполнения */
	void Clear();

	/* Деструктор */
	~TPrefix();

private:

	/* вектор потомков и путей в них */
	std::vector<std::pair<char, TPrefix*>> next;

	/* номер слова в таблице */
	unsigned int numberOfWord;

	/* последняя буква из файла */
	static char lastLetter;

	/* номер последнего добавленного слова */
	static unsigned int lastNumber;

	/* максимальная граница кол-ва слов перед созданием нового дерева */
	static const unsigned int border; //TODO а статик константу не надо капитализировать?
	
	static FILE read; //TODO а так можно?

	static FILE write;
};
