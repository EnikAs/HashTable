#include "listfunc.h"

int error = -1;

int ListInit (List* list, int elem_num)
{
    assert(list);

    list->lstelem = (ListElem*) calloc(elem_num, sizeof(ListElem)); 

    if (list->lstelem == NULL)
        return INCORRECT_CALLOC;

    else 
    {
        list->capacity = elem_num;

        for (int i = 0 ; i < elem_num ; i++)
        {
            list->lstelem[i].next = i + 1;
            list->lstelem[i].prev = -1;
        }

        list->lstelem[0].prev = -2;

        list->free = 0;
        list->head = 0;
        list->tail = 0;
        list->size = 0;

        return CORRECT_CALLOC;
    }
}

void SetListElem(List* list, int elem_num, list_t data, int next, int prev)
{
    assert(list);

    list->lstelem[elem_num].data = data;
    list->lstelem[elem_num].next = next;
    list->lstelem[elem_num].prev = prev;
}

int ListInsertTail(List* list, list_t value)
{
    assert(list);

    if (list->capacity <= list->size)
    {
        PRINT_LINE
        int corcheck = ListResize(list);
        if (corcheck == INCORRECT_REALLOC)
            return INCORRECT_REALLOC;
    }

    int tmp_pos = list->free;
    list->free = list->lstelem[tmp_pos].next;
    list->size += 1;

    list->lstelem[list->tail].next = tmp_pos;

    SetListElem(list, tmp_pos, value, -2, list->tail);// -2 means the end of list
    list->tail = tmp_pos;

    list->lstelem[list->head].prev = -2;

    $ListOkCheck(list, &error);

    return OK;
}

int ListInsertHead(List* list, list_t value)
{
    assert(list);

    $ListOkCheck(list, &error);

    list->sorted = false;

    if (list->capacity <= list->size)
    {
        int corcheck = ListResize(list);
        if (corcheck == INCORRECT_REALLOC)
            return INCORRECT_REALLOC;
    }

    int tmp_pos = list->free;
    list->free = list->lstelem[tmp_pos].next;
    list->size += 1;

    list->lstelem[list->head].prev = tmp_pos;

    SetListElem(list, tmp_pos, value, list->head, -2);// -2 means the start of list
    list->head = tmp_pos;

    list->lstelem[list->tail].next = -2;

    $ListOkCheck(list, &error);
    
    return OK;
}

int ListInsert (List* list, list_t value, int pos_init)// pos_init - элемент, после которого мы добавляем новый
{
    assert(list);

    $ListOkCheck(list, &error);

    list->sorted = false;

    if (list->capacity <= list->size)
    {
        int corcheck = ListResize(list);
        if (corcheck == INCORRECT_REALLOC)
            return INCORRECT_REALLOC;
    }
    int tmp_pos = list->free;
    list->free = list->lstelem[tmp_pos].next;
    list->size += 1;

    SetListElem(list, tmp_pos, value, list->lstelem[pos_init].next, pos_init);

    list->lstelem[list->lstelem[pos_init].next].prev = tmp_pos;
    list->lstelem[pos_init].next = tmp_pos;

    $ListOkCheck(list, &error);

    return OK;
}

int ListDelete (List* list, int pos_init)
{
    assert(list);

    $ListOkCheck(list, &error);
    list->size -= 1;

    if (pos_init == list->head)
    {
        list->lstelem[list->lstelem[pos_init].next].prev = -2;
    }
    else if (pos_init == list->tail)
    {
        list->lstelem[list->lstelem[pos_init].prev].next = -2;
    }
    else if (pos_init < list->capacity)
    {
    list->sorted = false;

    int tmp_next = list->lstelem[pos_init].next;
    int tmp_prev = list->lstelem[pos_init].prev;
    
    list->lstelem[tmp_prev].next = tmp_next;
    list->lstelem[tmp_next].prev = tmp_prev;

    list->lstelem[pos_init].next = list->free;
    list->lstelem[pos_init].prev = -1;
    list->free = pos_init;

    $ListOkCheck(list, &error);
        return OK;
    }
    else 
        return NOT_OK;
}

int ListResize (List* list)
{
    assert(list);

    list->capacity *= 2;
    list->lstelem = (ListElem*) realloc(list->lstelem, list->capacity * sizeof(ListElem));
    if (list->lstelem == NULL)
    {
        printf("memory realloc error");
        return INCORRECT_REALLOC;
    }

    for (int i = list->free ; i < list->capacity ; i++)
    {
        list->lstelem[i].next = i+1;
        list->lstelem[i].prev = -1;
    }

    $ListOkCheck(list, &error);
    return CORRECT_REALLOC;
}

int SlowSlowLinearaise (List* list)
{
    assert(list);

    if (list->sorted == true)
        return OK;

    int* tmp_data = (int*) calloc(list->size, sizeof(list_t));
    if (tmp_data == NULL)
    {
        printf("Calloc error!\n");
        return INCORRECT_CALLOC;
    }

    int tmp_pos = list->head;
    for (int i = 0 ; i < list->size; i++)
    {
        tmp_data[i] = list->lstelem[tmp_pos].data;
        list->lstelem[tmp_pos].data = 0;
        tmp_pos = list->lstelem[tmp_pos].next;
    }
    for (tmp_pos = 0 ; tmp_pos < list->capacity ; tmp_pos++)
    {
        if (tmp_pos == 0) // первый элемент 
        {
            SetListElem(list, tmp_pos, tmp_data[tmp_pos], tmp_pos + 1, -2);
        }

        else if (tmp_pos == list->size - 1) // последний элемент
        {
            SetListElem(list, tmp_pos, tmp_data[tmp_pos], -2, tmp_pos - 1);
        }

        else if (tmp_pos < list->size) // упорядоченные элементы
        {
            SetListElem(list, tmp_pos, tmp_data[tmp_pos], tmp_pos + 1, tmp_pos - 1);
        } 

        else if (tmp_pos >= list->size) // свободные элементы
        {
            SetListElem(list, tmp_pos, tmp_data[tmp_pos], tmp_pos + 1, -1);
        }
    }

    list->free = list->size;
    list->head = 0;
    list->tail = list->size - 1;
    list->sorted = true;
    

    return OK;
}

int SlowSlowLogicIntoPhys (List* list, int logic)
{
    assert(list);

    if (list->sorted == true)
        return logic - 1;

    logic -= 1;
    int tmp_pos = list->head;
    while (logic > 0)
    {
        tmp_pos = list->lstelem[tmp_pos].next;
        logic -= 1;
    }
    return tmp_pos;
}

int logic_error_check (List* list, int tmp_pos, int key)
{
    assert(list);

    if (key == PREV_CHECK)
    {
        if (list->lstelem[list->lstelem[tmp_pos].next].prev != tmp_pos && tmp_pos != list->tail)
        {
            printf("Logic error (incorrect prev)!");
            return LOGIC_PREV_ERROR;
        }
        return LOGIC_IS_OK;
    }
    else if (key == NEXT_CHECK)
    {
        if (list->lstelem[list->lstelem[tmp_pos].prev].next != tmp_pos && tmp_pos != list->head)
        {
            printf("Logic error (incorrect next)");
            return LOGIC_NEXT_ERROR;
        }
        return LOGIC_IS_OK;
    }
    else
        return INCORRECT_KEY;
    
}

int ListOkCheck (List* list, int* error)
{
    assert(list);

    if (list->size > list->capacity)
    {
        printf("Size is bigger than capacity!!!");
        *error = SIZE_ERROR;
        ListDump(list, *error);
    }

    int cunt = 0;
    int tmp_pos = list->head;

    while (1) // проверка на цикл
    {
        tmp_pos = list->lstelem[tmp_pos].next;
        cunt += 1;
        if (tmp_pos == -2)
            break;
        if (cunt > list->size + 2)
        {
            printf ("There is loop in this list!!!");
            *error = LOOP_ERROR;
            ListDump(list, *error);
        }
    }
    tmp_pos = list->head;
    for (int i = 0 ; i < list->size ; i++) // проверка на логические связи 
    {
        if (tmp_pos == list->head)              // первый элемент
        {
            if (list->lstelem[tmp_pos].prev != -2)
            {
                printf("Prev before head != -2");
                *error = HEAD_PREV_ERROR;
                ListDump(list, *error);
            }

            if (logic_error_check(list, tmp_pos, PREV_CHECK) == LOGIC_PREV_ERROR)
            {
                *error = LOGIC_PREV_ERROR;
                ListDump(list, *error);
            }
        }
        else if (tmp_pos == list->tail)         //последний элемент
        {
            if (list->lstelem[tmp_pos].next != -2)
            {
                printf("Next after tail != -2");
                *error = TAIL_NEXT_ERROR;
                ListDump(list, *error);
            }

            if (logic_error_check(list, tmp_pos, NEXT_CHECK) == LOGIC_NEXT_ERROR)
            {
                *error = LOGIC_NEXT_ERROR;
                ListDump(list, *error);
            }
        }                                       
        else                                    //остальные элементы
        {
            if (logic_error_check(list, tmp_pos, NEXT_CHECK) == LOGIC_NEXT_ERROR)
            {
                *error = LOGIC_NEXT_ERROR;
                ListDump(list, *error);
            }

            if (logic_error_check(list, tmp_pos, PREV_CHECK) == LOGIC_PREV_ERROR)
            {
                *error = LOGIC_PREV_ERROR;
                ListDump(list, *error);
            }
        }
        tmp_pos = list->lstelem[tmp_pos].next;
    }

    *error = LOGIC_IS_OK;
}

int EasyDump (List* list, FILE* log_file)
{
    assert(list);

    fprintf(log_file, "\n--------------------\n");
    for (int i = 0 ; i < list->capacity ; i++)
    {
        fprintf(log_file, "Elem number = %d, data = %d next = %d, prev = %d\n", i, list->lstelem[i].data, list->lstelem[i].next, list->lstelem[i].prev);
    }
    fprintf(log_file, "\n--------------------\n");

    return OK;
}

int ListDtor (List* list)
{
    assert(list);

    for (int i = 0 ; i < list->size ; i++)
    {
        list->lstelem[i].data = 0;
    }
    free(list);
}
int ListDump(List* list, int error)
{
    assert(list);

    if (error == -1) error = LOGIC_IS_OK;
    
    FILE* log_file = fopen("logfile.dot", "w");
    fprintf(log_file, "digraph G{\n");
    fprintf(log_file, "rankdir=TW;\n");// LR, UD, TW
    for (int i = 0 ; i < list->capacity ; i++)
    {
        if (list->lstelem[i].prev == -1)
        {
            fprintf(log_file, "%d [color = \"red\", style = \"filled\", fillcolor = \"red\","
                    "label =\" %d \\n value %d \n next %d, prev %d\",shape = \"hexagon\"];\n",
                    i, i, list->lstelem[i].data, list->lstelem[i].next, list->lstelem[i].prev);
        }
        else
            fprintf(log_file, "%d [color = \"green\", style = \"filled\", fillcolor = \"green\","
                    "label =\" %d \\n value %d \n next %d, prev %d\",shape = \"octagon\"];\n",
                    i, i, list->lstelem[i].data, list->lstelem[i].next, list->lstelem[i].prev);
    }

    for (int i = 0 ; i < list->capacity - 1 ; i++)
    {
        fprintf(log_file, "%d -> %d[style = \"invis\"];\n", i, i + 1);
    }

    char errorstr[1000] = "";
    switch(error)
    {
        case LOGIC_NEXT_ERROR:
            strcpy(errorstr, "Logic next error");
            fprintf(log_file, "322 [color = \"red\", style = \"filled\", fillcolor = \"red\", label =\" ERROR \\n %s\"];\n", errorstr);
            break;
        case LOGIC_PREV_ERROR:
            strcpy(errorstr, "Logic prev error");
            fprintf(log_file, "322 [color = \"red\", style = \"filled\", fillcolor = \"red\", label =\" ERROR \\n %s\"];\n", errorstr);
            break;
        case LOGIC_IS_OK:
            //strcpy(errorstr, "There are no errors!");
            //fprintf(log_file, "322 [color = \"red\", style = \"filled\", fillcolor = \"red\", label =\" ERROR \\n %s\"];\n", errorstr);
            break;
        case TAIL_NEXT_ERROR:
            strcpy(errorstr, "Logic error (incorrect next in tail)");
            fprintf(log_file, "322 [color = \"red\", style = \"filled\", fillcolor = \"red\", label =\" ERROR \\n %s\"];\n", errorstr);
            break;
        case HEAD_PREV_ERROR:
            strcpy(errorstr, "Logic error (incorrect prev in head)");
            fprintf(log_file, "322 [color = \"red\", style = \"filled\", fillcolor = \"red\", label =\" ERROR \\n %s\"];\n", errorstr);
            break;
        case SIZE_ERROR:
            strcpy(errorstr, "Size > capacity");
            fprintf(log_file, "322 [color = \"red\", style = \"filled\", fillcolor = \"red\", label =\" ERROR \\n %s\"];\n", errorstr);
            break;
        case LOOP_ERROR:
            strcpy(errorstr, "There is loop in this list!");
            fprintf(log_file, "322 [color = \"red\", style = \"filled\", fillcolor = \"red\", label =\" ERROR \\n %s\"];\n", errorstr);
            break;
    }

    fprintf(log_file, "-1 [label =\" List \\n size = %d, capacity = %d, head = %d, tail = %d, free = %d \"];\n",
                        list->size, list->capacity, list->head, list->tail, list->free);
    fprintf(log_file, "-100 [label =\"DATA\"];\n");
    fprintf(log_file, "-200 [label =\"FREE\"];\n");

    int tmp_pos = list->head;
    fprintf(log_file, "-100 -> %d\n", tmp_pos);
    for (int i = 1 ; i < list->size ; i++)
    {
        fprintf(log_file, "%d -> %d;\n", tmp_pos, list->lstelem[tmp_pos].next);
        tmp_pos = list->lstelem[tmp_pos].next;
    }

    tmp_pos = list->free;
    fprintf(log_file, "-200 -> %d\n", tmp_pos);
    for (int i = 0 ; i < list->capacity - list->size ; i++)
    {
        if (list->lstelem[tmp_pos].next == list->capacity)
            break;
        fprintf(log_file, "%d -> %d;\n", tmp_pos, list->lstelem[tmp_pos].next);
        tmp_pos = list->lstelem[tmp_pos].next;
    }

    fprintf(log_file, "}");
    fclose(log_file);

    system ("\"C:/Program Files/Graphviz/bin/dot.exe\" -Tpng logfile.dot -o list.png");

    return OK;
}

int ListHtmlDump (List* list)
{
    assert(list);

    FILE* log_html = fopen("log_file.html", "w");
    if (log_html == NULL)
        return FILE_NOT_OPENED;
    fprintf(log_html, "<pre>");
    fprintf(log_html, "<center><font size = \"7\">THIS IS LIST LOG !!!</font></center>");
    EasyDump(list, log_html);
    fprintf (log_html, "<center><img src = \"list.png\"></center>");
    fprintf(log_html, "</pre>");
    return OK;
}