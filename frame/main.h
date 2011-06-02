#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <linux/fb.h>
#include <jpeglib.h>

#define GRAY 0x00222222

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
    int m_x;
    int m_y;
    int flag;

    int button;
} MEVENT;

typedef struct button
{
    int button_x;
    int button_y;
    int button_height;
    int button_width;
    char button_pic[4096];

    struct button *next;
} BUTTON, *PBUTTON;

typedef struct jpeg_node
{
    u32_t *pjpeg;
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
int draw_rectangle(PFBDEV pfbdev, int x, int y, int w, int h, u32_t color);
JPEG_NODE *jpeg_decode(PFBDEV pfbdev, JPEG_NODE *p, const char *img_file);
int mouse_main(PFBDEV pfbdev);
FILE_NODE *load_init(FILE_NODE *head, const char *exname, const char *path);
int load_destroy(FILE_NODE *head);

JPEG_NODE *jpeg_create_link(PFBDEV pfbdev, JPEG_NODE *jpeg_head, const char *path);

/*in font.c*/
/*display a fong*/
int display_font(const char *code,int *len,int x,int y,PFBDEV pfbdev, u32_t color);
/* display string */
int display_string (const char *str, int x, int y, PFBDEV pfbdev,u32_t color);
/*int display_jpeg(JPEG_FILE *file, PFBDEV pfbdev, int x, int y, int w, int h);*/
int slide_display_jpeg(JPEG_NODE *headfile, PFBDEV pfbdev, int second);

// XiaoGuo TEST
int jpeg_thumb_disp(PFBDEV pfbdev, JPEG_NODE *headfile, int middle, int mov);

#endif
