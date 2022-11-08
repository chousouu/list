#include "list.h"

int main()
{
    struct List list = {};

    ListCtor_(list, 10);

    int err_code = 0; 

    ListPopBack(&list, &err_code);
    ListPopBack(&list, &err_code);

    ListPushBack (&list, 15);
    ListPushBack (&list, 15);
    ListPushBack (&list, 15);
    ListPushBack (&list, 15);

    ListPushBack (&list, 13);
    ListPushBack (&list, 14);
    ListPushBack (&list, 15);
    ListPushFront(&list, 23);
    ListPushFront(&list, 24);

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