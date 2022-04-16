#ifndef LISTFUNC_H_INCLUDED
#define LISTFUNC_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


typedef char* list_t;

const int LIST_IS_FULL = 1488;

#define OK_CHECK_OFF

#ifdef OK_CHECK_ON
#define $ListOkCheck(list) ListOkCheck(list)
#else
#define $ListOkCheck(list)    
#endif


#define PRINT_LINE printf("I'm at line %d at function %s\n", __LINE__, __func__);

enum for_fun
{
    OK = 1337,
    NOT_OK
};

enum equallity
{
    EQUAL = 0,
    NOT_EQUAL
};

enum open_file_check
{
    FILE_NOT_OPENED = 123,
    FILE_OPENED
};  

enum calloc_check
{
    INCORRECT_CALLOC = 228,
    CORRECT_CALLOC,
    INCORRECT_REALLOC,
    CORRECT_REALLOC
};

enum logic_check_key
{
    PREV_CHECK = 322,
    NEXT_CHECK,
    INCORRECT_KEY
};

enum ok_check
{
    LOGIC_NEXT_ERROR = 1488,
    LOGIC_PREV_ERROR,
    LOGIC_IS_OK,
    TAIL_NEXT_ERROR,
    HEAD_PREV_ERROR,
    SIZE_ERROR,
    LOOP_ERROR
};

struct ListElem 
{
    list_t data = NULL;
    int amount = 1;
    int next    = -1;
    int prev    = -1;
};

struct List
{
    ListElem* lstelem = NULL;
    int size          = 0;
    int capacity      = 0;
    int free          = 0;
    int head          = 0;
    int tail          = 0;
    bool sorted       = false;
};

struct buffer 
{
    char* buffer;
    int string_cunt = 0;
    int words_cunt = 0;
    int tmp_string_cunt = 0;
    int buffer_size = 0;
    int tmp_pos = 0;
};

struct Commands
{
    char* command;
    int lenght;
};

struct TableInputData
{
    buffer* buf;
    Commands* com;
};



int         ListInit                (List* list, int elem_num);

void        SetListElem             (List* list, int elem_num, list_t data, int next, int prev);

int         ListInsertTail          (List* list, list_t value);

int         ListInsertHead          (List* list, list_t value);

int         ListInsert              (List* list, list_t value, int pos_init = -1);

int         ListDelete              (List* list, int pos_init);

int         ListResize              (List* list);

int         SlowSlowLinearaise      (List* list);

int         SlowSlowLogicIntoPhys   (List* list, int logic);

int         logic_error_check       (List* list, int tmp_pos, int key);

int         ListOkCheck             (List* list);

int         EasyDump                (List* list, FILE* log_file = stdout);

int         ListDtor                (List* list);

int         ListDump                (List* list, FILE* log_file);

int         ListHtmlDump            (List* list);

int         RepeatCleaner           (List* list, int elem);

TableInputData*   init_all_commands (FILE* file_stream);

size_t      scanf_file_size         (FILE* input_file);

int         buffer_init             (buffer* buf, FILE* file_stream);

Commands*   commands_init           (buffer* buf);

int         get_all_commands        (Commands* com, buffer* buf);

int         get_one_command         (Commands* com, buffer* buf);

#endif