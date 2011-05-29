#ifndef _MAIN_H_
#define _MAIN_H_

#include <linux/fb.h>

typedef unsigned int u32_t;

typedef struct fbdev
{
    int fb;
    void *memo;
    struct fb_var_screeninfo fb_var;
    char dev[20];
} FBDEV, *PFBDEV;

typedef struct mevent
{
    int dx;
    int dy;
    int dz;

    int button;
} MEVENT;

typedef struct mousevent
{
    int x;
    int y;
    int h;
    int w;

    void (*function)(void *, ...);

    struct mousevent *next;
} MOUSEVENT, *PMOUSEVENT;

int fb_init(PFBDEV pFbdev);
int fb_destroy(PFBDEV pfbdev);
int fb_memset(void *addr, int c, size_t len);
int draw_pixel(PFBDEV pfbdev, int x, int y, u32_t color);
int mouse_main(PFBDEV pfbdev);
int fun_test(void);

#endif
