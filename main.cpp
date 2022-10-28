#include "list.h"

int main()
{
    struct List list = {};

    ListCtor(&list, 10);

    ListPushBack(&list, 1);
    ListPushBack(&list, 2);
    ListPushBack(&list, 3);
    ListPushBack(&list, 4);

    for(int i = 0; i < 10; i++)
    {
        printf("%d ", list.nodes[i].data);
    }
    printf("\n");

    ListInsertPhys(&list, 444, 3);

    for(int i = 0; i < 10; i++)
    {
        printf("%d ", list.nodes[i].data);
    }

    ListDtor(&list);
}