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

    //test
    int i, j;
    for (i = 0; i < 30; i++) 
    {
        for (j = 0; j < 100; j++) 
        {
            draw_pixel(&fbdev, 200+i, 200+j, 0x00ff0000);
        }
    }

    //end test

    //draw_pixel(&fbdev, 500, 500, 0x0000ff00);
    //mouse_main(&fbdev);

    fb_destroy(&fbdev);
    return 0;
}
