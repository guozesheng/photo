#include <stdio.h>
#include <string.h>
#include "main.h"

int main(int argc, const char *argv[])
{
    FBDEV fbdev;

    strcpy(fbdev.dev, "/dev/fb0");

    if (fb_init(&fbdev) == -1)
    {
        return -1;
    }

    JPEG_NODE *jpeg_head = NULL;
    jpeg_head = jpeg_create_link(&fbdev, jpeg_head, "./");
    //printf("%p\n", jpeg_head);
    //if (jpeg_head == NULL) 
    //{
        //printf("aaaaaaaaaaaa\n");
    //}
    //printf("%d, %d\n", (int)jpeg_head->jpeg_width, (int)jpeg_head->jpeg_height);
    slide_display_jpeg(jpeg_head, &fbdev, 1);

    ////test
    //int i, j;
    //for (i = 0; i < 30; i++) 
    //{
        //for (j = 0; j < 100; j++) 
        //{
            //draw_pixel(&fbdev, 200+i, 200+j, 0x00ff0000);
        //}
    //}

    ////end test

    //mouse_main(&fbdev);

    fb_destroy(&fbdev);
    return 0;
}
