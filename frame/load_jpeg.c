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

    tmp = malloc(sizeof(JPEG_NODE));

    if(NULL == tmp)
    {
        fprintf(stderr, "JPEG_NODE malloc error\n");
        return NULL;
    }

    if(NULL == head)
    {
        head = tmp;
        tmp->next = NULL;
        tmp->prev = NULL;
    }
    else
    {
        tmp->next = head;
        tmp->prev = NULL;
    }
    
    head = tmp;

    jpeg_decode(pfbdev, tmp, filename);

    return head;
}

JPEG_NODE *jpeg_create_link(PFBDEV pfbdev, JPEG_NODE *jpeg_head, const char *path)
{
    FILE_NODE *jpeg_file_head = NULL;

    jpeg_file_head = load_init(jpeg_file_head, ".jpg", path);

    while (jpeg_file_head != NULL) 
    {
        jpeg_head = jpeg_link_add(pfbdev, jpeg_head, jpeg_file_head->flname);
        jpeg_file_head = jpeg_file_head->next;
    }

    load_destroy(jpeg_file_head);
    
    return jpeg_head;
}

int jpeg_link_destroy(JPEG_NODE *head)
{
    JPEG_NODE *p;
    JPEG_NODE *n;

    p = head;

    while(NULL != p)
    {
        n = p;
        p = p->next;
        free(n);
    }

    return 0;
}

