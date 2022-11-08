#include "list.h"

int main()
{
    struct List list = {};

    ListCtor_(list, 10);

    int err_code = 0; 

    printf("SIZE : %d\n", list.size);
    ListPopBack(&list, &err_code);
    ListPopBack(&list, &err_code);

    ListPushBack(&list, 13);
    ListPushBack(&list, 14);
    ListPushBack(&list, 15);

    for(int i = 0; i < list.capacity; i++)
    {
        printf("[%d] - d:%d p:%d n:%d\n", i, list.nodes[i].data, list.nodes[i].prev, list.nodes[i].next);
    }

    printf("\n");
    printf("\n");

    for(int i = 16; i < 18; i++)
    {
        ListPushFront(&list, i);
    }

    for(int i = 0; i < list.capacity; i++)
    {
        printf("[%d] - d:%d p:%d n:%d\n", i, list.nodes[i].data, list.nodes[i].prev, list.nodes[i].next);
    }

    printf("\n TAIL = %d HEAD = %d\n", list.tail, list.head);

    FILE *graph = fopen("dotInput2.dot", "w");
    if(graph == NULL) 
    {
        printf("cannot open graph file!\n");
        return 0;
    }
    GraphDump(&list, graph);

    fclose(graph);

    ListDtor(&list);

    return 0;
}