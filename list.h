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

#ifdef DEBUG_MODE

struct ActionInfo 
{
    const char  *listname ;
    const char  *file_ctor;
    int          line_ctor;
};

#endif // DEBUGMODE

struct List 
{
    struct Node *nodes; 
    int capacity;
    int size;
    int head;
    int tail;
    int free;
    bool linear;

    #ifdef DEBUG_MODE
    struct ActionInfo Info;
    #endif //DEBUG_MODE
};

#ifdef DEBUG_MODE
    #define VAR_INFO , int line, const char *file, const char *name

    #define ListCtor_(list, X) ListCtor(&list, X, __LINE__, __FILE__, #list)
#else
    #define VAR_INFO  

    #define ListCtor_(&list, X) ListCtor(&list, X)
#endif

#define IF_ERR(ERROR, ERRCODE) do { if((ERROR)) problem_code |= (ERRCODE); } while(0)

#define FPRINT_ERR(ERR_CODE, ...) do {if(problem_code & ERR_CODE) {fprintf(logfile, __VA_ARGS__);}} while(0)

#define List_OK()                       \
do                                      \
{                                       \
    int err_code = ListVerify(list);    \
    ListDump(list, err_code);           \
    if(err_code) return err_code;       \
}                                       \
while(0) 

int ListCtor(struct List *list, int list_cap VAR_INFO);

int ListInsertPhys(struct List *list, type_t value, int pos);

int ListPushBack(struct List *list, type_t value); // v tail

int ListPushFront(struct List *list, type_t value); // v head

int ListPopFront(struct List *list, int *error_code);

int ListPopBack(struct List *list, int *error_code); // v tail

int ListPopPhys(struct List *list, int pos, int *error_code);

int ListResize(struct List *list, int cap);

int LogicToPhysAddr(List *list, long num);

void ListDtor(struct List *list);

//=================

int ListVerify(struct List *list);

void ListPrint(struct List *list);

void DecodeProblem(struct List *list, int problem_code);

void ListDump(struct List *list, int problem_code);

void GraphDump(struct List *list, FILE *graph);

#endif// LIST_H