/* 
 * disp-jpeg.c
 * by wang
 * 2011.5.29
 */

#include <stdlib.h>
#include <unistd.h>
#include "main.h"

JPEG_NODE *jpeg_display_zoom(JPEG_NODE *file, PFBDEV pfbdev, int w, int h)
{   
    int i,j;
    int dtw,dth;

    if(NULL == file) 
    {
       return NULL; 
    }
     
    JPEG_NODE *zoomfile = malloc(w*h*2);

    zoomfile->jpeg_width = w;
    zoomfile->jpeg_height = h;
    zoomfile->next = NULL;
    
    for(j = 0; j < h; j++) 
    {
        for(i = 0; i < w; i++) 
        {
            dtw = i*file->jpeg_width/w;
            dth = i*file->jpeg_height/h;
            dtw = dtw<dth?dtw:dth;
            zoomfile->pjpeg[i+j*w] = file->pjpeg[dtw+dtw*file->jpeg_width];
        }
    }

    return zoomfile;
}
int display_jpeg(JPEG_NODE *file, PFBDEV pfbdev, int x, int y, int w, int h)
{
    JPEG_NODE *zoomfile;
    int i, j;

    for(i = 0; i < pfbdev->fb_var.yres; ++i)
    {
        for (j = 0; j < pfbdev->fb_var.xres; ++j)
        {
            zoomfile = jpeg_display_zoom(file, pfbdev, w, h);
            draw_pixel(pfbdev, j, i, zoomfile->pjpeg[j + i*(pfbdev->fb_var.xres)]);
        }
    }

    return 0;
}

int slide_display_jpeg(JPEG_NODE *headfile, PFBDEV pfbdev, int second)
{
    JPEG_NODE *pfile;

    pfile = headfile;
    while(NULL != pfile)
    {
        display_jpeg(pfile, pfbdev, 0, 0, pfbdev->fb_var.xres, pfbdev->fb_var.yres);    
        sleep(second);
    }
    return 0;
}
