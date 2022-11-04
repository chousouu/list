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

int ListPopFront(struct List *list) //head ; 100% chastnie sluchai est'
{
    // check when tail == head

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

    return value;
}

int ListPopBack(struct List *list)
{
    // check when tail == head

    int value = list->nodes[list->tail].data;
    
    int prev_tail = list->nodes[list->tail].prev;

    list->nodes[prev_tail].next = 0;
    list->nodes[list->tail].prev = -1;
    
    list->nodes[list->tail].next = list->free;
    list->free = list->tail;

    list->nodes[list->tail].data = 0;

    list->tail = prev_tail;

    list->size--;

    return value;
}

int ListPopPhys(struct List *list, int pos)
{
    if(pos == list->tail)
    {
        return ListPopBack(list);
    }
    else if(pos == list->head)
    {
        return ListPopFront(list);
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

    return value;
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