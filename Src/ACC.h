#ifndef ACC_H
#define ACC_H

#include <fstream>
#include <cstdio>
#include <cstdlib>

// Количество битов в регистре
const long          BITS_IN_REGISTER = 16;

// Максимально возможное значение в регистре
const long          TOP_VALUE = ((long) 1 << 16) - 1;

// Диапазоны
const long          FIRST_QTR  = (TOP_VALUE / 4) + 1;
const long          HALF       = 2 * FIRST_QTR;
const long          THIRD_QTR  = 3 * FIRST_QTR;

// Количество символов алфавита
const long          NO_OF_CHARS = 256;
// Специальный символ EOF
const long          EOF_SYMBOL  = NO_OF_CHARS + 1;
// Всего символов в модели
const long          NO_OF_SYMBOLS = NO_OF_CHARS + 1;

// Порог частоты для масштабирования
const long          MAX_FREQUENCY = 16383;

class ACC {
    public:

        // Собственно адаптивное арифметическое кодирование
        void Compress (const char*, const char*);

        // Собственно адаптивное арифметическое декодирование
        void Decompress (const char*, const char*);

        // Инициализация адаптивной модели
        ACC();
        
        
    private:
        
        
        
        // Таблицы перекодировки
        unsigned char       indexToChar [NO_OF_SYMBOLS];
        int                 charToIndex [NO_OF_CHARS];
        
        // Таблицы частот
        int                 cumFreq [NO_OF_SYMBOLS + 1];
        int                 freq [NO_OF_SYMBOLS + 1];
        
        // Регистры границ и кода
        long                low, high;
        long                value;
        
        // Поддержка побитовых операций с файлами
        long                bitsToFollow;
        int                 buffer;
        int                 bitsToGo;
        int                 garbageBits;
        
        // Обрабатываемые файлы
        FILE                *out, *in;

        /* =============== Инициализация =============== */

        // Обновление модели очередным символом
        void                UpdateModel (int);

        // Инициализация побитового ввода
        void                StartInputingBits ();

        // Инициализация побитового вывода
        void                StartOutputingBits ();

        /* =============== Кодирование =============== */

        // Кодирование очередного символа
        void                EncodeSymbol (int);

        // Инициализация регистров границ и кода перед началом сжатия
        void                StartEncoding ();

        // Очистка побитового вывода
        void                DoneEncoding ();
        
        /* =============== Декодирование =============== */
        
        /* Инициализация регистров перед декодированием.
               Загрузка начала сжатого сообщения         */
        void                StartDecoding ();

        // Декодирование очередного символа
        int                 DecodeSymbol ();

        /* =============== Работа с буфером =============== */
        // Ввод очередного бита сжатой информации
        int                 InputBit ();

        // Вывод очередного бита сжатой информации
        void                OutputBit (int);

        // Очистка буфера побитового вывода
        void                DoneOutputingBits ();

        // Вывод указанного бита и отложенных ранее
        void                OutputBitPlusFollow (int);
};

#endif /* ACC_H */
