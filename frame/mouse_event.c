#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int print_hello(char *str, char *str2)
{
    printf("Hello, world!\n");
    printf("%s\n", str);
    printf("%s\n", str2);
    return 0;
}

int fun_use(PMOUSEVENT pevent)
{
    pevent->function("CCU", "Changchun");
    
    return 0;
}

int fun_test(PMOUSEVENT head, int x, int y, int h, int w)
{
    PMOUSEVENT newevent;

    newevent = malloc(sizeof(MOUSEVENT));

    head = newevent;
    newevent->next = NULL;
    newevent->x = 1;
    newevent->y = 2;
    newevent->h = 3;
    newevent->w = 4;
    newevent->function = (void *)print_hello;

    fun_use(newevent);

    free(newevent);
    
    return 0;
}
