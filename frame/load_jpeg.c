#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "main.h"

JPEG_FILE * jpeg_link_add(PFBDEV pfbdev, JPEG_FILE *head, const char *filename)
{
    JPEG_FILE *tmp;
    JPEG_FILE *p = head;

    tmp = malloc(sizeof(struct node));
    if(tmp == NULL)
        return NULL;
    tmp->pjpeg = jpeg_decode(pfbdev, filename);
    tmp->next = NULL;
    if(head == NULL)
        head = tmp;
    else
    {
        while(p->next != NULL)
            p = p->next;
        p->next = tmp;    
    }
    
    return head;
}

int jpeg_link_destroy(JPEG_FILE *head)
{
    JPEG_FILE *p;

    while(head != NULL)
    {
        p = head;
        head = head->next;
        free(p);
    }

    return 0;
}

int load_cmp_jpeg(const char *filename)
{
    int i, j;
    char jpeg_name[] = ".jpg";

    if (strlen(filename) >= 4) 
    {
        for (i = strlen(filename) - 1, j = 3; j >= 0 ; i--, j--) 
        {
            if (jpeg_name[j] != filename[i]) 
            {
                return 1;
            }
        }
    }
    else 
    {
        return 1;
    }

    return 0;
}

int load_init(PFBDEV pfbdev, JPEG_FILE *head)
{
    DIR *dp;

    dp = opendir("./");

    struct dirent *sdir;
    while ((sdir = readdir(dp)) != NULL)
    {
        if (sdir->d_type == DT_REG) 
        {
            if (!load_cmp_jpeg(sdir->d_name)) 
            {
                printf("%s\n", sdir->d_name);
            }
        }
    }
    
    return 0;
}
