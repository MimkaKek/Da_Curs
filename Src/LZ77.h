#pragma once
#include "Compressor.h"
class LZ77: public Compressor
{
public:
	LZ77();
	/* 
	 * Что делает: Сжимает данные
	 */
	void Compress() override;

	/* 
	 * Что делает: Расжимает данные
	 */
	void Decompress() override;

	/* 
	 * Что делает: Инициализирует поисковую структуру(словарь) необходимую для сжатия
	 */
	void InitEncode();

	/* 
	 * Что делает: Загружает словарь из потока ввода, начиная с позиции dictpos
	 */
	unsigned int LoadDict(unsigned int dictpos);

	/*
	 * Что делает: Удаляет из словаря сектор dectpos
	 */
	void DeleteData(unsigned int dictpos);

	/*
	 * Что делает: Хэширует данные
	 */
	void HashData(unsigned int dictpos, unsigned int bytestodo);

	/*
	 * Что делает: Ищет в словаре совпадение со строкой начиная с позиции dictpos
	 */
	void FindMatch(unsigned int dictpos, unsigned int startlen);

	/*
	 * Что делает: Ищет в словаре совпадение символов для текущего сектора
	 */
	void DictSearch(unsigned int dictpos, unsigned int bytestodo);

	virtual ~LZ77();
};