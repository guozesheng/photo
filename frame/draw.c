#include <stdio.h>
#include "main.h"

int draw_pixel(PFBDEV pfbdev, int x, int y, u32_t color)
{
    u8_t *pos = (u8_t *)pfbdev->memo + (x + y * pfbdev->fb_var.xres) * pfbdev->fb_var.bits_per_pixel / 8;

    if (x > 0 && x < pfbdev->fb_var.xres && y > 0 && y < pfbdev->fb_var.yres) 
    {
        switch (pfbdev->fb_var.bits_per_pixel)
        {
            //*((u32_t *)pfbdev->memo + x + y * pfbdev->fb_var.xres) = color;
            case 32:
            *(pos + 3) = color >> 24;
            case 24:
            *(pos + 2) = color >> 16;
            case 16:
            *(pos + 1) = color >> 8;
            return 0;
            default:
            return -1;
        }
    }
    
    return -1;
}
