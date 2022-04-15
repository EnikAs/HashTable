#include "HashTable.h"


int main()
{
    FILE* input_file = NULL;
    input_file = fopen("input.txt", "r");
    if (input_file == NULL)
        printf("Shit happends, incorrect file name!\n");

    HashTable* table = HashTableInit(2, 7, &HashFunc2);

    Commands* com = init_all_commands(input_file);

    PRINT_LINE

    for (int i = 0 ; i < com->buf->words_cunt; i++)
    {
        HashTableInsert(table, com[i].command, com[i].lenght);
    }

    PRINT_LINE

    HashTableDump(table);

    PRINT_LINE

    HashTableDtor(table);

    PRINT_LINE

    free(com->buf);
    free(com);

    return 0;
}