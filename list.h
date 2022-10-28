#ifndef LIST_H
#define LIST_H

#include "stdio.h"
#include "stdlib.h"

typedef int type_t;

const int POISON = 0xDEADDEAD;

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

int ListCtor(struct List *list, int list_cap);

int ListInsertPhys(struct List *list, type_t value, int pos);

int ListPushBack(struct List *list, type_t value); // v tail

int ListPushFront(struct List *list, type_t value); // v head

void ListDtor(struct List *list);
#endif// LIST_H