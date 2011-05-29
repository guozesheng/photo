#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "main.h"

#define C_WIDTH     10
#define C_HEIGHT    17
#define T___        0xffffffff
#define BORD        0x0
#define X___        0xffff

static u32_t cursor_pixel[C_WIDTH * C_HEIGHT] = 
{/*{{{*/
    BORD,T___,T___,T___,T___,T___,T___,T___,T___,T___,
    BORD,BORD,T___,T___,T___,T___,T___,T___,T___,T___,  
    BORD,X___,BORD,T___,T___,T___,T___,T___,T___,T___,
    BORD,X___,X___,BORD,T___,T___,T___,T___,T___,T___,
    BORD,X___,X___,X___,BORD,T___,T___,T___,T___,T___,
    BORD,X___,X___,X___,X___,BORD,T___,T___,T___,T___,
    BORD,X___,X___,X___,X___,X___,BORD,T___,T___,T___,
    BORD,X___,X___,X___,X___,X___,X___,BORD,T___,T___,
    BORD,X___,X___,X___,X___,X___,X___,X___,BORD,T___,
    BORD,X___,X___,X___,X___,X___,X___,X___,X___,BORD,
    BORD,X___,X___,X___,X___,X___,BORD,BORD,BORD,BORD,
    BORD,X___,X___,BORD,X___,X___,BORD,T___,T___,T___,
    BORD,X___,BORD,T___,BORD,X___,X___,BORD,T___,T___,
    BORD,BORD,T___,T___,BORD,X___,X___,BORD,T___,T___,
    T___,T___,T___,T___,T___,BORD,X___,X___,BORD,T___,
    T___,T___,T___,T___,T___,BORD,X___,X___,BORD,T___,
    T___,T___,T___,T___,T___,T___,BORD,BORD,T___,T___
};/*}}}*/
static u32_t cursor_save[C_WIDTH * C_HEIGHT] = {0};
int mouse_loop = 1;

int mouse_draw(PFBDEV pfbdev, int x, int y);
int mouse_parse(int fd, MEVENT *pmevent);
int mouse_save(PFBDEV pfbdev, int x, int y);
int mouse_restore(PFBDEV pfbdev, int x, int y);

int mouse_main(PFBDEV pfbdev)
{
    int fd;

    fd = open("/dev/input/mice", O_RDONLY | O_NONBLOCK);
    if (fd < 0) 
    {
        perror("open mice");
        return -1;
    }

    MEVENT mevent;
    int m_x = pfbdev->fb_var.xres / 2;
    int m_y = pfbdev->fb_var.yres / 2;

    mouse_draw(pfbdev, m_x, m_y);

    while (mouse_loop) 
    {
        if (mouse_parse(fd, &mevent) == 0) 
        {
            mouse_restore(pfbdev, m_x, m_y);

            m_x += mevent.dx;
            m_y += mevent.dy;
            m_x = (m_x < 0) ? 0 : m_x;
            m_y = (m_y < 0) ? 0 : m_y;
            m_x = (m_x > pfbdev->fb_var.xres - C_WIDTH) ? pfbdev->fb_var.xres - C_WIDTH : m_x;
            m_y = (m_y > pfbdev->fb_var.yres - C_HEIGHT) ? pfbdev->fb_var.yres - C_HEIGHT : m_y;

            mouse_draw(pfbdev, m_x, m_y);
            switch (mevent.button)
            {
                case 1:
                    printf("Left click!\n");
                    if (m_x>200 && m_x<230 && m_y>200 && m_y<300) 
                    {
                        mouse_loop = 0;
                    }
                    break;
                case 2:
                    printf("Right click!\n");
                    break;
                case 4:
                    printf("middle click!\n");
                    break;
                default: break;
            }
        }
        usleep(1000);
    }
    
    close(fd);
    return 0;
}

int mouse_restore(PFBDEV pfbdev, int x, int y)
{
    int i, j;

    for (j = 0; j < C_HEIGHT; j++) 
    {
        for (i = 0; i < C_WIDTH; i++) 
        {
            draw_pixel(pfbdev, x+i, y+j, cursor_save[i + j * C_WIDTH]);
        }
    }
    
    return 0;
}

int mouse_save(PFBDEV pfbdev, int x, int y)
{
    int i, j;

    for (j = 0; j < C_HEIGHT; j++) 
    {
        for (i = 0; i < C_WIDTH; i++) 
        {
            cursor_save[i + j * C_WIDTH] = *((u32_t *)pfbdev->memo + (x+i) + (y+j)*pfbdev->fb_var.xres);
        }
    }
    
    return 0;
}

int mouse_parse(int fd, MEVENT *pmevent)
{
    char buf[8];
    int n;

    n = read(fd, buf, 8);
    if (n > 0) 
    {
        pmevent->button = buf[0] & 0x07;
        pmevent->dx = buf[1];
        pmevent->dy = -buf[2];
        pmevent->dz = buf[3];
    }
    else 
    {
        return -1;
    }
    
    return 0;
}

int mouse_draw(PFBDEV pfbdev, int x, int y)
{
    int i, j;

    mouse_save(pfbdev, x, y);

    for (j = 0; j < C_HEIGHT; j++) 
    {
        for (i = 0; i < C_WIDTH; i++) 
        {
            if (cursor_pixel[i + j * C_WIDTH] != T___) 
            {
                draw_pixel(pfbdev, x+i, y+j, cursor_pixel[i + j * C_WIDTH]);
            }
        }
    }
    
    return 0;
}
