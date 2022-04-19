#include "HashTable.h"


int main()
{
    FILE* input_file = NULL;
    input_file = fopen("input.txt", "r");
    if (input_file == NULL)
        printf("Shit happends, incorrect file name!\n");

    HashTable* table = HashTableInit(20000, 100, &crc_hash);

    TableInputData* InpData = NULL;

    InpData = init_all_commands(input_file);

    //printf("%s\n", (char*)&(InpData->com[0].command));

    //HashTableInsert(table, (char*)&(InpData->com[0].command), InpData->com[0].lenght);

    //printf("%d - words counter in main before for cycle\n", InpData->buf->words_cunt);
    
    for (int i = 0 ; i < InpData->buf->words_cunt + 1; i++)
    {
        if (&(InpData->com[i].command) != NULL)
        {
            HashTableInsert(table, (char*)&(InpData->com[i].command), InpData->com[i].lenght);
        }
    }
    
    for(int k = 0 ; k < 10 ; k ++)
    {
        for (int i = 0 ; i < InpData->buf->words_cunt + 1; i++)
        {
            if (&(InpData->com[i].command) != NULL)
            {
                if (HashTableFind(table, (char*)&(InpData->com[i].command), InpData->com[i].lenght, PRESENCE)!=YES)
                    printf("jopa");
            }
        }
    }
    
    //HashTableDump(table);

    //char str[7] = "bitch.";
    //__m256i* word = HashTableWordTransform(str);
    //int am = HashTableFind(table, (char*)word, 6, AMOUNT);
    //printf("%d - ammount of \"%s\"\n", am, str);
    
    /*
    for (int i = 0 ; i < table->size ; i++)
    {
        printf("%d,", table->lists[i].size);
    }
    */

    HashTableDtor(table);

    free(InpData->com);
    free(InpData->buf->buffer);
    free(InpData->buf);
    free(InpData);

    return 0;
}