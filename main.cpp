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

    int words_number = InpData->buf->words_cunt;

    for (int i = 0 ; i < words_number; i++)
    {
       HashTableFind(table, (char*)&(InpData->com[i].command), InpData->com[i].lenght, INSERT);  
    }
    
    for(int k = 0 ; k < 10 ; k ++)
    {
        for (int i = 0 ; i < words_number; i++)
        {
            int check = HashTableFind(table, (char*)&(InpData->com[i].command), InpData->com[i].lenght, PRESENCE);
            //if (check != YES)
            //    printf("hopa");
        }
    }
    
    //char str[7] = "bitch.";
    //__m256i* word = HashTableWordTransform(str);
    //int am = HashTableFind(table, (char*)word, 6, AMOUNT);
    //printf("%d - ammount of \"%s\"\n", am, str);
    
    HashTableDtor(table);

    free(InpData->com);
    free(InpData->buf->buffer);
    free(InpData->buf);
    free(InpData);

    return 0;
}