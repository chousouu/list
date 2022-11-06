#ifndef LIST_H
#define LIST_H

#include "stdio.h"
#include "stdlib.h"

#define DEBUG_MODE

typedef int type_t;

const int POISON = 0xDEADDEAD;

enum ERRORS
{
    POP_ZERO_ELEM       = 1 << 0,
    NEGATIVE_SIZE       = 1 << 1,
    NEGATIVE_CAP        = 1 << 2,
    CAP_SMALLER_SIZE    = 1 << 3, 
    MEM_ALLOC_FAIL      = 1 << 4, 
    LIST_NULL           = 1 << 5,
};

struct Node 
{
    type_t data;
    int next;
    int prev; 
};

struct List 
{
    struct Node *nodes; 
    int capacity;
    int size; // shows actual size of list
    int head;
    int tail; //place of last elem of list (!= size) 
    int free;
    bool linear;
};

#define IF_ERR(ERROR, ERRCODE) do { if((ERROR)) problem_code |= (ERRCODE); } while(0)

#define List_OK()                       \
do                                      \
{                                       \
    int err_code = ListVerify(list);    \
    ListDump(list, err_code);           \
    if(err_code) return err_code;       \
}                                       \
while(0)

int ListCtor(struct List *list, int list_cap);

int ListInsertPhys(struct List *list, type_t value, int pos);

int ListPushBack(struct List *list, type_t value); // v tail

int ListPushFront(struct List *list, type_t value); // v head

int ListPopFront(struct List *list);

int ListPopBack(struct List *list); // v tail

int ListPopPhys(struct List *list, int pos);

int ListResize(struct List *list, int cap);

int LogicToPhysAddr(List *list, long num);

void ListDtor(struct List *list);

void ListDump(struct List *list, int problem_code);


#endif// LIST_H