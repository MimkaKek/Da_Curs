#include "Arithmetic.h"

Arithmetic::Arithmetic () {

    int i;
    
    for ( i = 0; i < NO_OF_CHARS; ++i) {
        charToIndex [i] = i + 1;
        indexToChar [i + 1] = i;
    }
    
    for ( i = 0; i <= NO_OF_SYMBOLS; ++i) {
        freq [i] = 1;
        cumFreq [i] = NO_OF_SYMBOLS - i;
    }

    in = nullptr;
    out = nullptr;
    chError = 0;
    freq [0] = 0;
}

//------------------------------------------------------------
void Arithmetic::UpdateModel (int symbol) {

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
void Arithmetic::StartInputingBits () {
    bitsToGo = 0;
    garbageBits = 0;
}

//------------------------------------------------------------
// Ввод очередного бита сжатой информации
int Arithmetic::InputBit () {

    int t;

    if (bitsToGo == 0) {
        buffer = getc (in);
        if (buffer == EOF) {
            ++garbageBits;
            if (garbageBits > BITS_IN_REGISTER - 2) {
                printf ("ERROR: Incorrect compress file!\n");
                chError = true;
                return 0;
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
void Arithmetic::StartOutputingBits () {
    buffer = 0;
    bitsToGo = 8;
}

//------------------------------------------------------------
// Вывод очередного бита сжатой информации
void Arithmetic::OutputBit (int bit) {

    buffer >>= 1;

    if (bit) {
        buffer |= 0x80;
    }

    --bitsToGo;

    if (bitsToGo == 0) {
        if(keys[0]) {
            std::cout << buffer;
        }
        else {
            putc(buffer, out);
        }
        bitsToGo = 8;
    }
}

//------------------------------------------------------------
// Очистка буфера побитового вывода
void Arithmetic::DoneOutputingBits () {
    if(keys[0]) {
        std::cout << (buffer >> bitsToGo);
    }
    else {
        putc(buffer >> bitsToGo, out);
    }
}

//------------------------------------------------------------
// Вывод указанного бита и отложенных ранее
void Arithmetic::OutputBitPlusFollow (int bit) {
    OutputBit (bit);
    while (bitsToFollow > 0) {
        OutputBit (!bit);
        --bitsToFollow;
    }
}

//------------------------------------------------------------
// Инициализация регистров границ и кода перед началом сжатия
void Arithmetic::StartEncoding () {
    low            = 0l;
    high           = TOP_VALUE;
    bitsToFollow   = 0l;
}

//------------------------------------------------------------
// Очистка побитового вывода
void Arithmetic::DoneEncoding () {

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
void Arithmetic::StartDecoding () {

    value = 0l;
    for ( int i = 0; i < BITS_IN_REGISTER; ++i) {
        value = 2 * value + InputBit ();
    }
    low = 0l;
    high = TOP_VALUE;
}

//------------------------------------------------------------
// Кодирование очередного символа
void Arithmetic::EncodeSymbol (int symbol) {

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
int Arithmetic::DecodeSymbol () {

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
        if(chError) {
            return 0;
        }
    }
    return symbol;
}

//------------------------------------------------------------
// Собственно адаптивное арифметическое кодирование
bool Arithmetic::Compress (const char *infile, const  char *outfile) {
    int ch, symbol;
    char tmp = 'A'; 
    
    in = fopen (infile, "r+b");
    if(!keys[0]) {
        out = fopen (outfile, "w+b");
    }

    if (in == nullptr || (out == nullptr && !keys[0])) {
        return false;
    }

    if(!keys[0]) {
        fwrite(&tmp, sizeof(char), 1, out);
    }
    else {
        std::cout << tmp;
    }
    
    unsigned long long savePos, sizeOfFile;
    savePos = ftell(in);
    fseek(in, 0, SEEK_END);
    sizeOfFile = ftell(in);
    fseek(in, savePos, SEEK_SET);
    if(!keys[0]) {
        fwrite(&sizeOfFile, sizeof(long long), 1, out);
    }
    else {
        std::cout << sizeOfFile;
    }

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
    if(!keys[0]) {
        fclose (out);
    }
    return true;
}

//------------------------------------------------------------
// Собственно адаптивное арифметическое декодирование
bool Arithmetic::Decompress (const char *infile, const char *outfile) {
    
    int symbol;
    unsigned char ch;
    char typeC = 0;
    unsigned long long oldSize = 0;
    in = fopen (infile, "r+b");
    if(!keys[0]) {
        out = fopen (outfile, "w+b");
    }
    if (in == nullptr || (out == nullptr && !keys[0])) {
        return false;
    }
    
    fread(&typeC, sizeof(char), 1, in);
    fread(&oldSize, sizeof(long long), 1, in);
    
    StartInputingBits ();
    StartDecoding ();
    for (;;) {
        symbol = DecodeSymbol ();
        
        if(chError) {
            return false;
        }
        
        if (symbol == EOF_SYMBOL) {
            break;
        }
        
        ch = indexToChar [symbol];
        
        if(!keys[5]) {
            if(keys[0]) {
                std::cout << ch;
            }
            else {        
                putc(ch, out);
            }
        }
        
        UpdateModel (symbol);
    }
    fclose (in);
    if(!keys[0]) {
        fclose (out);
    }
    return true;
}
