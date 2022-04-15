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

int HashTableDtor (HashTable* table)
{
    for (int i = 0 ; i < table->size ; i++)
    {
        ListDtor(&(table->lists[i]));
    }

    free(table->lists);
    free(table);

    return 0;
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
        ListDump(&(table->lists[i]), log_file);
    }

    fprintf(log_file, "}");
    fclose(log_file);

    int sys_ret = system ("dot -Tpng logfile.dot -o list.png");

    if (sys_ret != 0)
    {
        printf("Error was detected in \"system\" function");
    }

    return 0;
}

int HashTableRepeatCleaner (HashTable* table)
{
    for (int i = 0 ; i < table->size ; i++)
    {
        RepeatCleaner(&(table->lists[i]), table->lists[i].head);
    }

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
Commands* init_all_commands (FILE* file_stream)
{
    assert(file_stream);

    buffer* buf = (buffer*) calloc(1, sizeof(buffer));

    buffer_init(buf, file_stream);

    Commands* com = (Commands*) calloc(1, sizeof(Commands));

    com = commands_init(buf);
    
    get_all_commands(com, buf);

    com->buf = buf;

    //free(com);
    //free(buf->buffer);

    return com;
}

size_t scanf_file_size (FILE* input_file)
{
    struct stat file;
    if ( fstat(fileno(input_file), &file ) == -1 )
    {
        printf("FILE READING ERROR");
        return EOF;
    }

    return file.st_size;
}


int buffer_init (buffer* buf, FILE* file_stream)
{
    
    buf->buffer_size = scanf_file_size(file_stream);
    
    buf->buffer = (char*) calloc(buf->buffer_size, sizeof(char));
    
    if (buf->buffer == NULL)
    {
        printf("BUFFER MEMORY CALLOCATION ERROR\n");
    }
    
    buf->buffer_size = fread(buf->buffer, sizeof(char), buf->buffer_size, file_stream);

    for (int i = 0 ; i < buf->buffer_size ; i++)
    {
        if (buf->buffer[i] == '\0' || buf->buffer[i] == '\n' || buf->buffer[i] == ' ')
        {
            buf->words_cunt += 1;
        }
    }
        
    return 0;
}

Commands* commands_init (buffer* buf)
{
    Commands* com = (Commands*) calloc(buf->words_cunt + 1, sizeof(Commands));// СТАЛО
    if (com == NULL)
    {   
        printf("COMMANDS ARRAY MEMORY CALLOCATION ERROR!!!");
        return NULL;
    }

    return com;
}

int get_all_commands (Commands* com, buffer* buf)
{
    int end_check = -1;
    
    buf->words_cunt = 0;

    while (end_check != END_OF_FILE)
    {
        end_check = get_one_command(com, buf);
    }

    return 0;
}

#define CUR_SYM buf->buffer[buf->tmp_pos]

int get_one_command (Commands* com, buffer* buf)
{
    com[buf->words_cunt].command = ((char*)buf->buffer + buf->tmp_pos);

    while(CUR_SYM != '\n' && CUR_SYM != '\0' && CUR_SYM != ' ')
    {
        buf->tmp_pos += 1;
    }
    CUR_SYM = '\0';

    buf->tmp_pos += 1;

    com[buf->words_cunt].lenght = ((char*)buf->buffer + buf->tmp_pos) - com[buf->words_cunt].command;

    if (buf->tmp_pos >= buf->buffer_size)
    {
        return END_OF_FILE;
    }

    buf->words_cunt += 1;

    return -1;
}

#undef CUR_SYM