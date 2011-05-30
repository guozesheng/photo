#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <linux/fb.h>
#include <jpeglib.h>

typedef unsigned char u8_t;
typedef unsigned short u16_t;
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

typedef struct jpeg_node
{
    u16_t *pjpeg;
    JDIMENSION jpeg_width;
    JDIMENSION jpeg_height;

    struct jpeg_node *prev;
    struct jpeg_node *next;
} JPEG_NODE;

typedef struct file_node
{
    char flname[4096];

    struct file_node *next;
} FILE_NODE;

int fb_init(PFBDEV pfbdev);
int fb_destroy(PFBDEV pfbdev);
int fb_memset(void *addr, int c, size_t len);
int draw_pixel(PFBDEV pfbdev, int x, int y, u32_t color);
JPEG_NODE *jpeg_decode(PFBDEV pfbdev, JPEG_NODE *p, const char *img_file);
int mouse_main(PFBDEV pfbdev);
FILE_NODE *load_init(FILE_NODE *head, const char *exname, const char *path);
int load_destroy(FILE_NODE *head);

/*in font.c*/
/*display a fong*/
int display_font(const char *code,int *len,int x,int y,PFBDEV pfbdev, u32_t color);
/* display string */
int display_string (const char *str, int x, int y, PFBDEV pfbdev,u32_t color);
/*int display_jpeg(JPEG_FILE *file, PFBDEV pfbdev, int x, int y, int w, int h);*/
int slide_display_jpeg(JPEG_NODE *headfile, PFBDEV pfbdev, int second);

#endif
