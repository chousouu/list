#include "list.h"

int main()
{
    struct List list = {};

    ListCtor(&list, 10);

    ListPushBack(&list, 1);
    ListPushBack(&list, 2);
    ListPushBack(&list, 3);
    ListPushBack(&list, 4);
    ListPushBack(&list, 55);


    for(int i = 0; i < 10; i++)
    {
        printf("[%d] - d:%d p:%d n:%d ||", i, list.nodes[i].data, list.nodes[i].prev, list.nodes[i].next);
    }
    printf("\n");
    printf("\n");

    ListPopFront(&list);

    for(int i = 0; i < 10; i++)
    {
        printf("[%d] - d:%d p:%d n:%d ||", i, list.nodes[i].data, list.nodes[i].prev, list.nodes[i].next);
    }

    printf("\n");

    ListDtor(&list);
}