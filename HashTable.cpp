#include "HashTable.h"

HashTable* HashTableInit (int TableSize, int ListSize, long long (*hashfunc) (char* word, int leng))
{
    assert(hashfunc);

    HashTable* table = (HashTable*) calloc(1, sizeof(HashTable));
    if (table == NULL)
        printf("hash table's memory callocation error!");

    table->size = TableSize;
    table->hashfunc = hashfunc;

    table->lists = (List*) calloc(TableSize, sizeof(List));
    if (table->lists == NULL)
        printf("table's lists memory callocation error!");

    for(int i = 0 ; i < TableSize ; i++)
    {
        ListInit( &(table->lists[i]), ListSize );
    }

    return table;
}

long long HashFunc1 (char* ch, int leng)  //это просто единичка
{
    return 1;
}

long long HashFunc2 (char* ch, int leng)  //это просто ASCII-код первой буквы
{
    long long hash = 0;
    hash += *ch;
    return hash;
}

long long HashFunc3 (char* ch, int leng)  //это просто длина слова
{
    return leng;
}

long long HashFunc4 (char* ch, int leng)  //сумма ASCII кодов слова
{
    long long hash = 0;
    for (int i = 0 ; i < leng ; i++)
    {
        hash += ch[i];
    }
    return hash;
}

int HashTableInsertFast (HashTable* table, char* word, int wordsize)
{
    int listnum = 0;
    listnum = table->hashfunc(word, wordsize)%table->size;

    ListInsertTail( &(table->lists[listnum]), word);

    return 0;
}

int HashTableFind (HashTable* table, char* word, int wordsize, int key)
{
    int listnum = table->hashfunc(word, wordsize)%table->size;
    int cur_lst_elem = table->lists[listnum].head;
    
    for (int i = 0 ; i < table->lists[listnum].size ; i++)
    {
        int cur_strlen = strlen(table->lists[listnum].lstelem[cur_lst_elem].data);
        int max_strlen = (cur_strlen > wordsize) ? cur_strlen:wordsize;
        
        if (strncmp(word, table->lists[listnum].lstelem[cur_lst_elem].data, max_strlen) == EQUAL)
        {
            switch(key)
            {
                case PRESENCE:
                    return YES;

                case AMOUNT:
                    return table->lists[listnum].lstelem[cur_lst_elem].amount; 

                case INSERT:
                    table->lists[listnum].lstelem[cur_lst_elem].amount += 1;
                    return 0;

                default:
                    printf("INCORRECT KEY IN HASHTABLEFIND FUNCTION!!!!!!!!!!!!!!!\n");
                    return -1;
            }
        }
        cur_lst_elem = table->lists[listnum].lstelem[cur_lst_elem].next;
    }

    switch(key)
    {
        case PRESENCE:
            return NO;

        case AMOUNT:
            return 0; 

        case INSERT:
            ListInsertTail( &(table->lists[listnum]), word);
            return 0;
        
        default:
            printf("INCORRECT KEY IN HASHTABLEFIND FUNCTION!!!!!!!!!!!!!!!\n");
            return -1;
    }

    return 0;
}

int HashTableInsert (HashTable* table, char* word, int wordsize)
{
    HashTableFind(table, word, wordsize, INSERT);
    return 0;
}

int HashTableDump (HashTable* table)
{
    FILE* log_file = fopen("logfile.dot", "w");
    fprintf(log_file, "digraph G{\n");
    fprintf(log_file, "rankdir=UD;\n");// LR, UD, TW

    for (int i = 0 ; i < table->size ; i++)
    {
        ListDump(&(table->lists[i]), -1, log_file);
    }

    fprintf(log_file, "}");
    fclose(log_file);

    system ("dot -Tpng logfile.dot -o list.png");
}

int HashTableRepeatCleaner (HashTable* table)
{
    for (int i = 0 ; i < table->size ; i++)
    {
        RepeatCleaner(&(table->lists[i]), table->lists[i].head);
    }
}