#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED

#include "listfunc.h"

struct HashTable
{
    List* lists   = {};
    int   size    = 0;
    long long (*hashfunc) (char* word, int leng) = NULL;
};

enum FindInTableKey
{
    PRESENCE = 228,
    AMOUNT,
    INSERT
};

enum PresenceCheck
{
    YES = 1,
    NO = -1,
};

HashTable* HashTableInit (int TableSize, int ListSize, long long (*hashfunc) (char* word, int leng));

long long HashFunc1 (char* ch, int leng);

long long HashFunc2 (char* ch, int leng);  //это просто ASCII-код первой буквы

long long HashFunc3 (char* ch, int leng);  //это просто длина слова

long long HashFunc4 (char* ch, int leng);  //сумма ASCII кодов слова

int HashTableInsertFast (HashTable* table, char* word, int wordsize);

int HashTableFind (HashTable* table, char* word, int wordsize, int key);

int HashTableInsert (HashTable* table, char* word, int wordsize);

int HashTableDump (HashTable* table);

int HashTableRepeatCleaner (HashTable* table);


#endif