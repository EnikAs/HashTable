#include "HashTable.h"


int main()
{
    FILE* input_file = NULL;
    input_file = fopen("input.txt", "r");
    if (input_file == NULL)
        printf("Shit happends, incorrect file name!\n");

    HashTable* table = HashTableInit(1024, 200, &murmurHash);

    TableInputData* InpData = NULL;

    InpData = init_all_commands(input_file);

    for (int i = 0 ; i < InpData->buf->words_cunt + 1; i++)
    {
        if (InpData->com[i].command != NULL)
        {
            HashTableInsert(table, InpData->com[i].command, InpData->com[i].lenght);
        }
    }

    for(int k = 0 ; k < 10 ; k ++)
    {
        int counter = 0;
        
        for (int i = 0 ; i < InpData->buf->words_cunt + 1; i++)
        {
            if (InpData->com[i].command != NULL)
            {
                if (HashTableFind(table, InpData->com[i].command, InpData->com[i].lenght, PRESENCE) != YES)
                {
                    counter += 1;
                    printf("%d ", counter);
                }
            }
        }
    }

    HashTableDtor(table);

    free(InpData->com);
    free(InpData->buf->buffer);
    free(InpData->buf);
    free(InpData);

    return 0;
}