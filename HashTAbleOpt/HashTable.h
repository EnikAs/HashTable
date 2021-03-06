#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

#include "listfunc.h"

struct HashTable
{
    List* lists   = {};
    int   size    = 0;
    unsigned long long (*hashfunc) (char* word, int leng) = NULL;
};

struct buffer 
{
    char* buffer;
    int string_cunt = 0;
    int words_cunt = 0;
    int tmp_string_cunt = 0;
    int buffer_size = 0;
    int tmp_pos = 0;
};

struct Commands
{
    char* command;
    int lenght;
};

struct TableInputData
{
    buffer* buf;
    Commands* com;
};


enum FindInTableKey
{
    PRESENCE = 228,
    AMOUNT,
    INSERT,
    DELETE
};

enum PresenceCheck
{
    YES = 1,
    NO = -1,
};

const int END_OF_FILE = 322;

HashTable* HashTableInit            (int TableSize, int ListSize, unsigned long long (*hashfunc) (char* word, int leng));

int HashTableDtor                   (HashTable* table);

unsigned long long HashFunc1        (char* word, int leng);

unsigned long long HashFunc2        (char* word, int leng);  //это просто ASCII-код первой буквы

unsigned long long HashFunc3        (char* word, int leng);  //это просто длина слова

unsigned long long HashFunc4        (char* word, int leng);  //сумма ASCII кодов слова

unsigned long long HashFunc5        (char* word, int leng);  //тут все уже не так просто (используется ROR)

unsigned long long murmurHash       (char * word, int leng);

int HashTableInsertFast             (HashTable* table, char* word, int wordsize);

int HashTableFind                   (HashTable* table, char* word, int wordsize, int key);

int HashTableInsert                 (HashTable* table, char* word, int wordsize);

int HashTableDump                   (HashTable* table);

int HashTableRepeatCleaner          (HashTable* table);

TableInputData*   init_all_commands (FILE* file_stream);

size_t      scanf_file_size         (FILE* input_file);

int         buffer_init             (buffer* buf, FILE* file_stream);

Commands*   commands_init           (buffer* buf);

int         get_all_commands        (Commands* com, buffer* buf);

int         get_one_command         (Commands* com, buffer* buf);

unsigned long long ROR              (unsigned long long H);

#endif