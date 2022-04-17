#include "HashTable.h"

HashTable* HashTableInit (int TableSize, int ListSize, unsigned long long (*hashfunc) (char* word, int leng))
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

unsigned long long HashFunc1 (char* word, int leng)  //это просто единичка
{
    return 1;
}

unsigned long long HashFunc2 (char* word, int leng)  //это просто ASCII-код первой буквы
{
    long long hash = 0;
    hash += *word;
    return hash;
}

unsigned long long HashFunc3 (char* word, int leng)  //это просто длина слова
{
    return leng;
}

unsigned long long HashFunc4 (char* word, int leng)  //это просто сумма ASCII кодов слова
{
    long long hash = 0;

    for (int i = 0 ; i < leng ; i++)
    {
        hash += word[i];
    }
    return hash;
}

unsigned long long HashFunc5 (char* word, int leng)  //тут все уже не так просто (используется ROR)
{
    unsigned long long H0 = word[0];

    for (int i = 1 ; i < leng -1 ; i++)
    {
        H0 = ROR(H0) ^ (word[i+1]);
    }

    return H0;
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

                case DELETE:
                    ListDelete(&(table->lists[listnum]), cur_lst_elem);
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
        
        case DELETE:
            printf("I cant delete element \"%s\" - it's not found", word);
            return 0;

        default:
            printf("INCORRECT KEY IN HASHTABLEFIND FUNCTION!!!!!!!!!!!\n");
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
        printf("Error was detected in \"system\" function\n");
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

TableInputData* init_all_commands (FILE* file_stream)
{
    assert(file_stream);

    buffer* buf = (buffer*) calloc(1, sizeof(buffer));

    buffer_init(buf, file_stream);

    Commands* com = commands_init(buf);
    
    get_all_commands(com, buf);

    TableInputData* InpTable = (TableInputData*) calloc(1, sizeof(TableInputData));

    InpTable->buf = buf;
    InpTable->com = com;

    return InpTable;
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
    
    int i_to_save = 0;

    bool flag = false;

    for (int i = 0 ; i < buf->buffer_size ; i++)
    {
        if (buf->buffer[i] == '\n' || buf->buffer[i] == '\0' || buf->buffer[i] == ' ')
        {
            if (flag == false)
            {
                buf->words_cunt += 1;
                flag = true;
                buf->buffer[i_to_save] = buf->buffer[i];
                i_to_save += 1;
            }
        }
        else
        {
            flag = false;

            buf->buffer[i_to_save] = buf->buffer[i];
            i_to_save += 1;
        }
    }

    buf->words_cunt += 1;

    buf->buffer_size = i_to_save;

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

FILE* jopa = fopen("shitfile.txt", "w");

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

    buf->words_cunt += 1;

    if (buf->tmp_pos >= buf->buffer_size)
    {
        return END_OF_FILE;
    }

    return -1;
}

#undef CUR_SYM

 unsigned long long murmurHash (char * word, int leng)
{
  const unsigned int m = 0x5bd1e995;
  const unsigned int seed = 0;
  const int r = 24;

  unsigned long long h = seed ^ leng;

  const unsigned char * data = (const unsigned char *)word;
  unsigned long long k;

  while (leng >= 4)
  {
      k  = data[0];
      k |= data[1] << 8;
      k |= data[2] << 16;
      k |= data[3] << 24;

      k *= m;
      k ^= k >> r;
      k *= m;

      h *= m;
      h ^= k;

      data += 4;
      leng -= 4;
  }

  switch (leng)
  {
    case 3:
      h ^= data[2] << 16;
    case 2:
      h ^= data[1] << 8;
    case 1:
      h ^= data[0];
      h *= m;
      break;
    default:
        break;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

unsigned long long ROR (unsigned long long H)
{
    unsigned long long H1 = 0;
    unsigned long long H2 = 0;

    H1 = H >> 1;
    H2 = H << 63;

    H = H1 | H2;

    return H;
}