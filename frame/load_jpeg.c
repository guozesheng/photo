#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "main.h"

JPEG_NODE * jpeg_link_add(PFBDEV pfbdev, JPEG_NODE *head, const char *filename)
{
    JPEG_NODE *tmp;
    JPEG_NODE *p = head;

    tmp = malloc(sizeof(JPEG_NODE));

    if(tmp == NULL)
        return NULL;

    tmp->pjpeg = jpeg_decode(pfbdev, filename);
    tmp->next = NULL;

    if(head == NULL)
    {
        head = tmp;
    }
    else
    {
        while(p->next != NULL)
            p = p->next;
        p->next = tmp;    
    }
    
    return head;
}

int jpeg_link_destroy(JPEG_NODE *head)
{
    JPEG_NODE *p;

    while(head != NULL)
    {
        p = head;
        head = head->next;
        free(p);
    }

    return 0;
}

