#include "list.h"

FILE *logfile = NULL;

int ListVerify(struct List *list)
{
    if(list == NULL)
    {
        return LIST_NULL;
    }
    int problem_code = 0; 

    IF_ERR(list->size < 0, NEGATIVE_SIZE);

    IF_ERR(list->capacity < 0, NEGATIVE_CAP);

    IF_ERR(list->size > list->capacity, CAP_SMALLER_SIZE);

    IF_ERR(list->nodes == NULL, MEM_ALLOC_FAIL);

    return problem_code;
}

void DecodeProblem(struct List *list, int problem_code)
{
    FPRINT_ERR(LIST_NULL, "LIST IS NULL\n");

    FPRINT_ERR(NEGATIVE_SIZE, "NEGATIVE SIZE (= %d)\n", list->size);

    FPRINT_ERR(NEGATIVE_CAP, "NEGATIVE CAPACITY (= %d)\n", list->capacity);

    FPRINT_ERR(CAP_SMALLER_SIZE, "CAP < SIZE (%d < %d)\n", list->capacity, list->size);

    FPRINT_ERR(MEM_ALLOC_FAIL, "COULDNT ALLOCATE MEMORY FOR LIST NODES\n");

    FPRINT_ERR(POP_ZERO_ELEM, "ZERO ELEM POP\n");

    return;
}

void ListDump(struct List *list, int problem_code)
{
    logfile = fopen("log.txt", "a");

    if(logfile == NULL)
    {
        printf("Cannot open the file!\n");
        return;
    }
    fprintf(logfile, "=========================\n");
    fprintf(logfile, "%s at %s (%d):\n", list->Info.listname, list->Info.file_ctor, list->Info.line_ctor);
    
    if(problem_code)
    {
        DecodeProblem(list, problem_code);

        fprintf(logfile,"{\n");

            fprintf(logfile, "size = %d, cap = %d\n", list->size, list->capacity);

            fprintf(logfile,"{\t\n");

                ListPrint(list);

            fprintf(logfile,"}\t\n");

        fprintf(logfile,"}\n");
    }

    //graph dump 
    //dot dotInput.dot -Tpng -o dumps/Img.png


    fclose(logfile);

    return;
}

void GraphDump(struct List *list, FILE *graph)
{
    fprintf(graph, "digraph g {\n");

    fprintf(graph, 
                "fontname= \"Helvetica,Arial,sans-serif\"\n"
                "node [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                "edge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
                "graph [rankdir = \"LR\"];\n"
                "node [fontsize = \"16\" shape = \"ellipse\"];\n"
                "edge [];\n"
            );

    //create nodes
    for(int i = 0; i < list->capacity; i++)
    {
        fprintf(graph, 
                    " \"node%d\" [\n"
                    "label = \"<phys> id: %d | <data> data: %d | { <prev> prev: %d| <next> next: %d}\"\n"
                    "shape = \"record\"\n"
                    "];\n"
               , i, i, list->nodes[i].data, list->nodes[i].prev, list->nodes[i].next);
    }

    for(int i = 0; i < list->capacity; i++)
    {
        fprintf(graph, 
                    "\"node%d\":next -> \"node%d\":prev\n",
                    i, list->nodes[i].next
                );
        if(list->nodes[i].prev > -1)
        fprintf(graph, 
                    "\"node%d\":prev -> \"node%d\":next\n",
                    i, list->nodes[i].prev
                );
    }

    fprintf(graph,  
                "HEAD[shape=\"hexagon\",style=\"filled\",fillcolor=\"green\"];\n"
                "HEAD -> \"node%d\"\n", list->head
            );

    fprintf(graph,  
                "TAIL[shape=\"rpromoter\",style=\"filled\",fillcolor=\"red\"];\n"
                "TAIL -> \"node%d\"\n", list->tail
            );

    for(int i = 0; i < list->capacity; i++)
    {
        fprintf(graph, 
                    " \"node%d\" -> \"node%d\"[weight=1000, style=\"invis\"]\n", i, list->nodes[i].next  
                );
        if(list->nodes[i].prev > -1)
        fprintf(graph, 
                " \"node%d\" -> \"node%d\"[weight=1000, style=\"invis\"]\n", i, list->nodes[i].prev  
            );
    }

    fprintf(graph, "}");

    return;
}


void ListPrint(struct List *list)
{
    int i = 0;
    int print_to = list->capacity;

    fprintf(logfile, "N  |\tPREV\t|\t DATA\t |\tNEXT\n");
    for(; i < print_to; i++)
    {
        fprintf(logfile, "%-3d|%-12d|%-12d|%-12d", i, list->nodes[i].prev, list->nodes[i].data, list->nodes[i].next);
        if(i == list->head)
        {
            fprintf(logfile, "<----HEAD");
        }
        if(i == list->tail)
        {
            fprintf(logfile, "<----TAIL");
        }
        fprintf(logfile, "\n");
    }
}

int ListCtor(struct List *list, int list_cap VAR_INFO)
{   
    Node *tmp = (Node *)realloc(list->nodes, sizeof(Node) * list_cap);

    if(tmp == NULL)
    {
        printf("Could not reallocate memory\n");
        return MEM_ALLOC_FAIL;
    }
    
    *list = 
    {
        .nodes = tmp, 
        .capacity = list_cap,
        .head = 0,
        .tail = 0,
        .free = 1,
        .linear = 1,
    };
        #ifdef DEBUG_MODE
        list->Info.listname  = name;
        list->Info.file_ctor = file;
        list->Info.line_ctor = line;
        #endif

    for(int i = 0; i < list_cap; i++)
    {
        list->nodes[i].data = POISON;
        list->nodes[i].next = (i + 1) % list_cap;
        list->nodes[i].prev = -1;
    }
    list->nodes[0].data = 0;

    List_OK();
    
    return 1;
}

int ListInsertPhys(struct List *list, type_t value, int pos) // (pos - pos_next)  ==== > (pos - dest - pos_next)
{
    List_OK();

    if(list->free < 1)
    {
        ListResize(list, list->capacity * 2);        
    }

    int dest = list->free;

    if(pos == list->tail) 
    {
        return ListPushBack(list, value);
    }
    if (pos == list->head)
    {
        return ListPushFront(list, value);
    }
    
    list->free = list->nodes[dest].next; //next .free

    list->nodes[dest].data = value;

    // dest <=> pos_next
    list->nodes[dest].next = list->nodes[pos].next;
    list->nodes[list->nodes[pos].next].prev = dest;

    //pos <=> dest
    list->nodes[pos].next = dest; 
    list->nodes[dest].prev = pos;  

    list->linear = 0;
    list->size++;

    List_OK();

   return dest; 
}

int ListPushBack(struct List *list, type_t value)
{
    List_OK();

    if(list->free < 1)
    {
        ListResize(list, list->capacity * 2);        
    }

    int dest = list->free;
    
    list->free = list->nodes[dest].next;

    if(list->tail == 0)
    {
        list->tail = dest;
        list->head = dest;

        list->nodes[dest].data = value;
        list->nodes[dest].next = 0;
        list->nodes[dest].prev = 0;
        list->size++;

        return dest;
    }   

    list->nodes[dest].data = value;
    list->nodes[dest].next = 0;
    list->nodes[dest].prev = list->tail;

    list->nodes[list->tail].next = dest;
    list->tail = dest;
    
    list->size++;

    List_OK();

    return dest;
}

int ListPushFront(struct List *list, type_t value)
{
    List_OK();

    if(list->free < 1)
    {
        ListResize(list, list->capacity * 2);        
    }

    int dest = list->free;

    list->free = list->nodes[dest].next;

    if (list->tail == 0)
    {
        list->tail = dest;
        list->head = dest;
        list->nodes[dest].data = value;
        list->nodes[dest].next = 0;
        list->nodes[dest].prev = 0;
        list->size++;

        return dest;
    }

    list->nodes[dest].data = value;

    list->nodes[dest].next = list->head;
    list->nodes[list->head].prev = dest;

    list->nodes[dest].prev = 0;

    list->head = dest;
    list->linear = 0;

    list->size++;

    List_OK();

    return dest;
}

int ListPopFront(struct List *list, int *error_code) //head 
{   
    List_OK();

    if(list->size < 1)
    {
        *error_code = POP_ZERO_ELEM;
        ListDump(list, *error_code);
        return *error_code;
    }

    int value = list->nodes[list->head].data;

    // head_next.prev <=> head.prev (0)
    list->nodes[list->nodes[list->head].next].prev = list->nodes[list->head].prev;
    list->nodes[list->nodes[list->head].prev].next = list->nodes[list->head].next;    

    //reset info about head
    list->nodes[list->head].data = 0;
    list->nodes[list->head].prev = -1;

    // HEAD now will be free elem
    list->nodes[list->head].next = list->free;
    list->free = list->head;

    //change head
    list->head = list->nodes[list->head].next;

    list->size--;

    List_OK();

    return value;
}

int ListPopBack(struct List *list, int *error_code)
{
    List_OK();

    if(list->size < 1)
    {
        *error_code = POP_ZERO_ELEM;
        ListDump(list, *error_code);
        return *error_code;
    }

    if(list->tail == list->head)
    {
        return ListPopFront(list, error_code);
    }

    int value = list->nodes[list->tail].data;
    
    int prev_tail = list->nodes[list->tail].prev;

    list->nodes[prev_tail].next = 0;
    list->nodes[list->tail].prev = -1;
    
    list->nodes[list->tail].next = list->free;
    list->free = list->tail;

    list->nodes[list->tail].data = 0;

    list->tail = prev_tail;

    list->size--;

    List_OK();

    return value;
}

int ListPopPhys(struct List *list, int pos, int *error_code)
{
    List_OK();

    if(list->size < 1)
    {
        *error_code = POP_ZERO_ELEM;
        ListDump(list, *error_code);
        return *error_code;
    }

    if(pos == list->tail)
    {
        return ListPopBack(list, error_code);
    }
    else if(pos == list->head)
    {
        return ListPopFront(list, error_code);
    }

    int value = list->nodes[pos].data;

    // prev_pos <=> next_pos
    printf("PREV NEXT %d ; next prev %d\n", list->nodes[list->nodes[pos].prev].next, list->nodes[list->nodes[pos].next].prev);

    list->nodes[list->nodes[pos].prev].next = list->nodes[pos].next;
    list->nodes[list->nodes[pos].next].prev = list->nodes[pos].prev;

    printf("PREV NEXT %d ; next prev %d\n", list->nodes[list->nodes[pos].prev].next, list->nodes[list->nodes[pos].next].prev);

    //clear pos
    list->nodes[pos].data = 0;
    list->nodes[pos].prev = -1;

    //change free to popped elem
    list->nodes[pos].next = list->free;
    list->free = pos;
    
    list->linear = 0;
    list->size--;

    List_OK();

    return value;
}

int ListResize(struct List *list, int cap)
{
    Node *tmp = (Node *)realloc(list->nodes, sizeof(Node) * (cap));
    
    if(tmp == NULL) 
    {
        printf("ERROR could not realloc\n"); 
        return 0;
    } 
    
    list->nodes = tmp;
    
    if(cap > list->capacity)
    {
        for(int i = list->capacity; i < cap; i++)
        {                                                   
            list->nodes[i].data = 0;                        
            list->nodes[i].next = (i + 1) % cap;
            list->nodes[i].prev = -1;              
        }
        list->free = list->capacity;
    } 
    list->capacity = cap;

    return 1;  
}

int LogicToPhysAddr(List *list, long num)
{
    if(list->linear)
    {
        return num + list->head - 1;
    }
    
    long elem  = list->head;
    long steps = 1;

    for(; steps < num; elem = list->nodes[elem].next, steps++);
    
    return elem;
}

void ListDtor(struct List *list)
{
    free(list->nodes);
    list->capacity = POISON;
    list->head     = POISON;
    list->tail     = POISON;
    list->free     = POISON;
    list->linear   = 0; 

    return;
}