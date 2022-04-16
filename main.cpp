#include "HashTable.h"


int main()
{
    FILE* input_file = NULL;
    input_file = fopen("input.txt", "r");
    if (input_file == NULL)
        printf("Shit happends, incorrect file name!\n");

    HashTable* table = HashTableInit(100, 100, &HashFunc2);

    TableInputData* InpData = NULL;

    InpData = init_all_commands(input_file);

    printf("%d - words counter in main before for cycle\n", InpData->buf->words_cunt);
    for (int i = 0 ; i < InpData->buf->words_cunt; i++)
    {
        if (InpData->com[i].command != NULL)
        {
            HashTableInsert(table, InpData->com[i].command, InpData->com[i].lenght);
        }
    }

    //HashTableDump(table);
    
    char str[7] = "bitch.";
    int am = HashTableFind(table, str, 7, AMOUNT);
    printf("%d - ammount of %s\n", am, str);
    
    HashTableDtor(table);

    free(InpData->com);
    free(InpData->buf);
    free(InpData);

    return 0;
}