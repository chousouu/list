#include "list.h"

int ListCtor(struct List *list, int list_cap)
{
    //verif: listcap < 1? & ... 

    Node *tmp = (Node *)realloc(list->nodes, sizeof(Node) * list_cap);

    if(tmp == NULL)
    {
        //verif; 
        printf("couldnt reallocate memory\n");
        return 0;
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


    for(int i = 0; i < list_cap; i++)
    {
        list->nodes[i].data = 0;
        list->nodes[i].next = (i + 1) % list_cap;
        list->nodes[i].prev = -1; 
    }

    list->nodes[0].next = 0;

    //ListOK();
    
    return 1;
}

int ListInsertPhys(struct List *list, type_t value, int pos) // (pos - pos_next)  ==== > (pos - dest - pos_next)
{
    // millions of debug if's

    //RESIZE IF FREE =-1
    int dest = list->free;

    if(pos == list->tail) //is this needed or should be removed since ListPushBack()?
    {
        list->tail = dest;
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

   //verif 

   return dest; 
}

int ListPushBack(struct List *list, type_t value)
{
    //RESIZE IF FREE =-1

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

    return dest;
}


int ListPushFront(struct List *list, type_t value)
{
    //RESIZE IF FREE =-1
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

    return dest;
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