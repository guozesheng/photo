#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "main.h"

MEVENT mevent;
pthread_mutex_t mouse_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, const char *argv[])
{
    FBDEV fbdev;
    pthread_t t_mouse_id;

    strcpy(fbdev.dev, "/dev/fb0");

    if (fb_init(&fbdev) == -1)
    {
        return -1;
    }

    pthread_create(&t_mouse_id, NULL, (void *)mouse_main, (void *)&fbdev);

    JPEG_NODE *jpeg_head = NULL;
    jpeg_head = jpeg_create_link(&fbdev, jpeg_head, "../pic/");
    //slide_display_jpeg(jpeg_head, &fbdev, 1);
    jpeg_thumb_disp(&fbdev, jpeg_head, 0, 0);

    fb_destroy(&fbdev);
    return 0;
}
