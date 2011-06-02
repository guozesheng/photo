#include <stdio.h>
#include "main.h"

int draw_pixel(PFBDEV pfbdev, int x, int y, u32_t color)
{
    //*((u32_t *)pfbdev->memo + x + y * pfbdev->fb_var.xres) = color;
    u8_t *pos = (u8_t *)pfbdev->memo + (x + y * pfbdev->fb_var.xres) * pfbdev->fb_var.bits_per_pixel / 8;
    if (x > 0 && x < pfbdev->fb_var.xres && y > 0 && y < pfbdev->fb_var.yres) 
    {
        switch (pfbdev->fb_var.bits_per_pixel)
        {
            case 32:
                *(pos + 3) = color >> 24;
            case 24:
                *(pos + 2) = color >> 16;
            case 16:
                *(pos + 1) = color >> 8;
            case 8: 
                *pos = color;
                return 0;
            default:
                return -1;
        }
    }
    
    return -1;
}

int draw_rectangle(PFBDEV pfbdev, int x, int y, int w, int h, u32_t color)
{
    int i, j;

    for (j = 0; j < h; j++) 
    {
        for (i = 0; i < w; i++) 
        {
            draw_pixel(pfbdev, x+i, y+j, color);
        }
    }
    
    return 0;
}
