#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "main.h"

int load_cmp(const char *filename, const char *exname)
{
    int i, j;

    for (i = strlen(filename) - 1, j = strlen(exname) - 1; j >= 0 ; i--, j--) 
    {
        if (exname[j] != filename[i]) 
        {
            return 1;
        }
    }

    return 0;
}

FILE_NODE *load_init(FILE_NODE *head, const char *exname, const char *path)
{
    DIR *dp;
    char name_tmp[4096];

    dp = opendir(path);

    struct dirent *sdir;
    while ((sdir = readdir(dp)) != NULL)
    {
        if (sdir->d_type == DT_REG) // This is a regular file.
        {
            if (!load_cmp(sdir->d_name, exname)) 
            {
                FILE_NODE *p = malloc(sizeof(FILE_NODE));
                if (p != NULL)
                {
                    strcpy(name_tmp, path);
                    strcat(name_tmp, sdir->d_name);
                    strcpy(p->flname, name_tmp);
                    p->next = head;
                    head = p;
                }
            }
        }
    }
    
    return head;
}

int load_destroy(FILE_NODE *head)
{
    FILE_NODE *p;

    while (head != NULL) 
    {
        p = head;
        head = head->next;
        free(p);
    }
    
    return 0;
}
