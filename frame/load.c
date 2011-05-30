#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "main.h"

int load_cmp(const char *filename, const char *exname)
{
    int i, j;
    char jpeg_name[] = ".jpg";

    filename = "abcdefg.jpg";
    exname = ".jpg";

    for (i = strlen(filename) - 1, j = strlen(exname) - 1; j >= 0 ; i--, j--) 
    {
        if (exname[j] != filename[i]) 
        {
            return 1;
        }
    }

    return 0;
}

FILENAME *load_init(FILENAME *head, const char *exname, const char *path)
//int load_init(PFBDEV pfbdev, JPEG_FILE *head)
{
    DIR *dp;

    dp = opendir(path);

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
    
    return head;
}
