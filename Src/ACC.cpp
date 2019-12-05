#include "ACC.h"

ACC::ACC () {

    int i;
    
    for ( i = 0; i < NO_OF_CHARS; ++i) {
        charToIndex [i] = i + 1;
        indexToChar [i + 1] = i;
    }
    
    for ( i = 0; i <= NO_OF_SYMBOLS; ++i) {
        freq [i] = 1;
        cumFreq [i] = NO_OF_SYMBOLS - i;
    }
    
    freq [0] = 0;
}

//------------------------------------------------------------
void ACC::UpdateModel (int symbol) {

    int i;
    int chI, chSymbol;
    int cum;
    
    // проверка на переполнение счетчика частоты
    if (cumFreq [0] == MAX_FREQUENCY) {
        cum = 0;
        // масштабирование частот при переполнении
        for ( i = NO_OF_SYMBOLS; i >= 0; --i) {
            freq [i] = (freq [i] + 1) / 2;
            cumFreq [i] = cum;
            cum += freq [i];
        }
    }
    
    for (i = symbol; freq [i] == freq [i - 1]; --i);

    if (i < symbol) {
        chI                       = indexToChar [i];
        chSymbol                  = indexToChar [symbol];
        indexToChar [i]           = chSymbol;
        indexToChar [symbol]      = chI;
        charToIndex [chI]         = symbol;
        charToIndex [chSymbol]    = i;
    }

    // обновление значений в таблицах частот
    freq [i] += 1;
    while (i > 0) {
        --i;
        cumFreq [i] += 1;
    }
}

//------------------------------------------------------------
// Инициализация побитового ввода
void ACC::StartInputingBits () {
    bitsToGo = 0;
    garbageBits = 0;
}

//------------------------------------------------------------
// Ввод очередного бита сжатой информации
int ACC::InputBit () {

    int t;

    if (bitsToGo == 0) {
        buffer = getc (in);
        if (buffer == EOF) {
            ++garbageBits;
            if (garbageBits > BITS_IN_REGISTER - 2) {
                printf ("Ошибка в сжатом файле\n");
                exit (-1);
            }
        }
        bitsToGo = 8;
    }

    t = buffer & 1;
    buffer >>= 1;
    --bitsToGo;

    return t;
}

//------------------------------------------------------------
// Инициализация побитового вывода
void ACC::StartOutputingBits () {
    buffer = 0;
    bitsToGo = 8;
}

//------------------------------------------------------------
// Вывод очередного бита сжатой информации
void ACC::OutputBit (int bit) {

    buffer >>= 1;

    if (bit) {
        buffer |= 0x80;
    }

    --bitsToGo;

    if (bitsToGo == 0) {
        putc (buffer, out);
        bitsToGo = 8;
    }
}

//------------------------------------------------------------
// Очистка буфера побитового вывода
void ACC::DoneOutputingBits () {
    putc(buffer >> bitsToGo, out);
}

//------------------------------------------------------------
// Вывод указанного бита и отложенных ранее
void ACC::OutputBitPlusFollow (int bit) {
    OutputBit (bit);
    while (bitsToFollow > 0) {
        OutputBit (!bit);
        --bitsToFollow;
    }
}

//------------------------------------------------------------
// Инициализация регистров границ и кода перед началом сжатия
void ACC::StartEncoding () {
    low            = 0l;
    high           = TOP_VALUE;
    bitsToFollow   = 0l;
}

//------------------------------------------------------------
// Очистка побитового вывода
void ACC::DoneEncoding () {

    ++bitsToFollow;
    if (low < FIRST_QTR) {
        OutputBitPlusFollow(0);
    }
    else {
        OutputBitPlusFollow(1);
    }
}

//------------------------------------------------------------
/* Инициализация регистров перед декодированием.
   Загрузка начала сжатого сообщения
*/
void ACC::StartDecoding () {

    value = 0l;
    for ( int i = 0; i < BITS_IN_REGISTER; ++i) {
        value = 2 * value + InputBit ();
    }
    low = 0l;
    high = TOP_VALUE;
}

//------------------------------------------------------------
// Кодирование очередного символа
void ACC::EncodeSymbol (int symbol) {

    long range;
    
    // пересчет значений границ
    range = (long) (high - low) + 1;
    high  = low + (range * cumFreq [symbol - 1]) / cumFreq [0] - 1;
    low   = low + (range * cumFreq [symbol]    ) / cumFreq [0];
    // выдвигание очередных битов
    for (;;) {
        if (high < HALF) {
            OutputBitPlusFollow (0);
        }
        else if (low >= HALF) {
            OutputBitPlusFollow (1);
            low -= HALF;
            high -= HALF;
        }
        else if (low >= FIRST_QTR && high < THIRD_QTR) {
            ++bitsToFollow;
            low -= FIRST_QTR;
            high -= FIRST_QTR;
        }
        else
            break;
            
        // сдвиг влево с "втягиванием" очередного бита
        low = 2 * low;
        high = 2 * high + 1;
    }
}

//------------------------------------------------------------
// Декодирование очередного символа
int ACC::DecodeSymbol () {

    long range;
    int cum, symbol;

    // определение текущего масштаба частот
    range = (long) (high - low) + 1;
    // масштабирование значения в регистре кода
    cum = (int) ((((long) (value - low) + 1) * cumFreq [0] - 1) / range);
    // поиск соответствующего символа в таблице частот
    for (symbol = 1; cumFreq [symbol] > cum; symbol++);
    // пересчет границ
    high = low + (range * cumFreq [symbol - 1]) / cumFreq [0] - 1;
    low  = low + (range * cumFreq [symbol]    ) / cumFreq [0];
    // удаление очередного символа из входного потока
    for (;;) {

        if (high < HALF) {}
        else if (low >= HALF) {
            value -= HALF;
            low   -= HALF;
            high  -= HALF;
        }
        else if (low >= FIRST_QTR && high < THIRD_QTR) {
            value -= FIRST_QTR;
            low   -= FIRST_QTR;
            high  -= FIRST_QTR;
        }
        else
            break;

        // сдвиг влево с "втягиванием очередного бита
        low   = 2 * low;
        high  = 2 * high + 1;
        value = 2 * value + InputBit ();
    }
    return symbol;
}

//------------------------------------------------------------
// Собственно адаптивное арифметическое кодирование
void ACC::Compress (const char *infile, const  char *outfile) {
    int ch, symbol;
    
    in = fopen ( infile, "r+b");
    out = fopen ( outfile, "w+b");
    if (in == NULL || out == NULL) {
        return;
    }

    ch = 'A';
    putc(ch, out);
    
    unsigned long long savePos, sizeOfFile;
    savePos = ftell(in);
    fseek(in, 0, SEEK_END);
    sizeOfFile = ftell(in);
    fseek(in, savePos, SEEK_SET);

    fwrite(&sizeOfFile, sizeof(long long), 1, out);
    StartOutputingBits ();
    StartEncoding ();
    for (;;) {
        ch = getc (in);
        if (ch == EOF) {
            break;
        }
        symbol = charToIndex [ch];
        EncodeSymbol (symbol);
        UpdateModel (symbol);
    }
    EncodeSymbol (EOF_SYMBOL);
    DoneEncoding ();
    DoneOutputingBits ();
    fclose (in);
    fclose (out);
}

//------------------------------------------------------------
// Собственно адаптивное арифметическое декодирование
void ACC::Decompress (const char *infile, const char *outfile) {
    
    int ch, symbol;
    
    in = fopen ( infile, "r+b");
    out = fopen ( outfile, "w+b");
    if (in == NULL || out == NULL) {
        return;
    }

    StartInputingBits ();
    StartDecoding ();
    for (;;) {
        symbol = DecodeSymbol ();
        if (symbol == EOF_SYMBOL) {
            break;
        }
        ch = indexToChar [symbol];
        putc ( ch, out);
        UpdateModel (symbol);
    }
    fclose (in);
    fclose (out);
}
