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

#define CUR_DATA table->lists[listnum].lstelem[cur_lst_elem].data

int HashTableFind (HashTable* table, char* word, int wordsize, int key)
{
    int listnum = table->hashfunc((char*)word, wordsize)%table->size;
    int cur_lst_elem = table->lists[listnum].head;

    //PRINT_LINE

    for (int i = 0 ; i < table->lists[listnum].size ; i++)
    {
        if (wordsize == table->lists[listnum].lstelem[cur_lst_elem].lenght)
        {
            __m256i comp_res = _mm256_cmpeq_epi8(*((__m256i*)word), *((__m256i*)(CUR_DATA)));

            if (_mm256_movemask_epi8(comp_res) == EQUAL_MASK)
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

            ListInsertTail( &(table->lists[listnum]), word, wordsize);
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

__m256i* HashTableWordTransform (char* word)
{
    __m256i* var = (__m256i*) aligned_alloc(32, sizeof(__m256i) * 1);

    int cunt = 0;

    for(; cunt < strlen(word) ; cunt++)
    {
        *((char*)var + cunt) = word[cunt];
    }

    for(; cunt < 32 ; cunt++)
    {
        *((char*)var + cunt) = 0;
    }
    
    PRINT_LINE

    return var;
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

    int sys_ret = system("dot -Tpng logfile.dot -o list.png");

    if (sys_ret != 0)
    {
        printf("Error was detected in \"system\" function\n");
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
    Commands* com = (Commands*) aligned_alloc(sizeof(Commands), buf->words_cunt * sizeof(Commands));// СТАЛО
  
    //memset(com, 0, buf->words_cunt * sizeof(Commands));
  
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
    com[buf->words_cunt].command = _mm256_setzero_si256();

    int m_pos = 0;

    char* com_ptr = (char*)&(com[buf->words_cunt].command);

    while(CUR_SYM != '\n' && CUR_SYM != '\0' && CUR_SYM != ' ')
    {
        *(com_ptr + m_pos) = CUR_SYM;
        m_pos += 1;
        buf->tmp_pos += 1;
    }

    com[buf->words_cunt].lenght = m_pos;

    buf->tmp_pos += 1;

    buf->words_cunt += 1;

    if (buf->tmp_pos >= buf->buffer_size)
    {
        return END_OF_FILE;
    }

    return 0;
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

long long compute_golden_lut_intel(char* pTbl, uint32_t n)
{
    uint64_t R = 1;
    for (uint32_t i = 0; i < n << 1; ++i)
    {
        pTbl[i] = (uint32_t)R;
        R = _mm_crc32_u64(R, 0);
    }
}

unsigned long long crc_hash(char* src, int leng)
{
	uint64_t answ = 0;

	for (int i = 0; i != 4; i++)
		answ = _mm_crc32_u64(answ, ((uint64_t *)src)[i]);

	return (unsigned long long)answ;
}