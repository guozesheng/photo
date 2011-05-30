#include <stdio.h>
#include "main.h"

int draw_pixel(PFBDEV pfbdev, int x, int y, u32_t color)
{
    if (x > 0 && x < pfbdev->fb_var.xres && y > 0 && y < pfbdev->fb_var.yres) 
    {
        *((u32_t *)pfbdev->memo + x + y * pfbdev->fb_var.xres) = color;
    }
    
    return 0;
}
