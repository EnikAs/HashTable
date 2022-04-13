#include "HashTable.h"


int main()
{
    HashTable* table = HashTableInit(2, 7, &HashFunc2);
    char a[] = "roma";
    char b[] = "suka";
    char c[] = "lox";
    char d[] = "suka1";
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, a, 5);
    HashTableInsert(table, b, 5);
    HashTableInsert(table, b, 5);
    HashTableInsert(table, b, 5);
    HashTableInsert(table, b, 5);
    HashTableInsert(table, b, 5);
    HashTableInsert(table, c, 4);
    HashTableInsert(table, c, 4);
    HashTableInsert(table, c, 4);
    HashTableInsert(table, d, 6);
    HashTableInsert(table, d, 6);
    HashTableInsert(table, d, 6);
    HashTableInsert(table, d, 6);
    
    HashTableDump(table);

    return 0;
}