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
    //int dtw,dth;

    if(NULL == file) 
    {
       return NULL; 
    }
    JPEG_NODE *zoomfile = malloc(sizeof(JPEG_NODE));
   
    //zoomfile->pjpeg = malloc(w*h*2);
    //zoomfile->jpeg_width = w;
    //zoomfile->jpeg_height = h;

    //for(j = 0; j < h; j++) 
    //{
        //for(i = 0; i < w; i++) 
        //{
            //dtw = i*file->jpeg_width/w;
            //dth = j*file->jpeg_height/h;
            //zoomfile->pjpeg[i+j*w] = file->pjpeg[dtw+dth*file->jpeg_width];
        //}
    //}
    
    double dt, dw ,dh;
    dw = (double)file->jpeg_width/(double)w;
    dh = (double)file->jpeg_height/(double)h;
    dt = dw>dh?dw:dh;
    zoomfile->jpeg_width = (double)file->jpeg_width/dt;
    zoomfile->jpeg_height = (double)file->jpeg_height/dt;


    zoomfile->pjpeg = malloc(zoomfile->jpeg_width*zoomfile->jpeg_height*pfbdev->fb_var.bits_per_pixel/8);

    for(j = 0; j < zoomfile->jpeg_height; j++) 
    {
        for(i = 0; i < zoomfile->jpeg_width; i++) 
        {
            zoomfile->pjpeg[i+j*zoomfile->jpeg_width] = file->pjpeg[(int)(i*dt)+(int)(j*dt)*file->jpeg_width];
        }
    }

    return zoomfile;
}
int display_jpeg(JPEG_NODE *file, PFBDEV pfbdev, int x, int y, int w, int h)
{
    JPEG_NODE *zoomfile;
    int i, j;
    

    zoomfile = jpeg_display_zoom(file, pfbdev, w, h);

    for(i = 0; i < zoomfile->jpeg_height; ++i)
    {
        for (j = 0; j < zoomfile->jpeg_width; ++j)
        {
            draw_pixel(pfbdev, j, i, zoomfile->pjpeg[j + i*(zoomfile->jpeg_width)]);
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
        display_jpeg(pfile, pfbdev, 0, 0, 200, 100);    
        pfile = pfile->next;
        sleep(second);
    }
    return 0;
}
