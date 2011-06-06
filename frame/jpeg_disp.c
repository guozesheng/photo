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
            draw_pixel(pfbdev, j+x, i+y, zoomfile->pjpeg[j + i*(zoomfile->jpeg_width)]);
        }
    }

    return 0;
}

int slide_display_jpeg(JPEG_NODE *headfile, PFBDEV pfbdev, int second)
{
    JPEG_NODE *pfile;

    pfile = headfile;
    while(1)
    {
        if (pfile == NULL) 
        {
            pfile = headfile;
        }
        display_jpeg(pfile, pfbdev, 0, 0, pfbdev->fb_var.xres, pfbdev->fb_var.yres);    
        display_jpeg(pfile, pfbdev, 0, 0, 200, 100);    
        pfile = pfile->next;
        sleep(second);
    }
    return 0;
}

 /*
int jpeg_thumb_disp(PFBDEV pfbdev, JPEG_NODE *headfile, int middle, int mov) // By XiaoGuo@0601:1911
{   // mov: -1:left; 1:right; 0:none
    int thum_num = 4;                           // The number of the thumbnail
    float elemt = 6.0;                          // the number of copies to split the wide
    float times = 0.714;                        // zoom levels
    float thumbw = pfbdev->fb_var.xres / elemt; // The width of the thumbnail
    float thumbw_space = thumbw / 14;            // The width of the space
    float thumbw_button = thumbw / 2;           // The width of the button
    float thumbh = pfbdev->fb_var.yres / elemt; // The height of the thumbnail
    float thumby = pfbdev->fb_var.yres - thumbh - thumbw_space; // The y coordinate of the thumbnail
    int middlex, middley, middlew, middleh;
    JPEG_NODE *pthumb = headfile;
    int i, thumbx;

    middlew = pfbdev->fb_var.xres * times;
    middleh = pfbdev->fb_var.yres * times;

    middlex = (pfbdev->fb_var.xres - middlew) / 2;
    middley = thumbw_space;

    draw_rectangle(pfbdev, 0, thumby, pfbdev->fb_var.xres-1, thumbh, GRAY); // The background of the thumbnail
    draw_rectangle(pfbdev, thumbw_space, thumby, thumbw_button, thumbh, 0x00440000);
    draw_rectangle(pfbdev, pfbdev->fb_var.xres - thumbw_space - thumbw_button, thumby, thumbw_button, thumbh, 0x00440000);

    for (i = 0; i < thum_num && headfile != NULL; i++) 
    {
        //thumbx = thumbw_space + thumbw_button + thumbw + (thumbw_space + thumbw) * i;
        thumbx = thumbw_space + thumbw_button + thumbw_space + (thumbw_space + thumbw) * i;
        display_jpeg(pthumb, pfbdev, thumbx, thumby, thumbw, thumbh);
        pthumb = pthumb->next;
    }

    display_jpeg(headfile, pfbdev, middlex, middley, middlew, middleh);
    
    return 0;
}
// */

int jpeg_thumb_disp(PFBDEV pfbdev, JPEG_NODE *headfile, int middle, int mov) // By XiaoGuo@0601:1911
{   // mov: -1:left; 1:right; 0:none
    //int thum_num = 4;                             // The number of the thumbnail
    float times = 0.75;                           // zoom levels
    float times_space = 80.0;                     // space zoom levels
    float thumbw;                                 // The width of the thumbnail
    float space;                                  // The width of the space
    float thumbw_button;                          // The width of the button
    float thumbh;                                 // The height of the thumbnail
    float thumby;                                 // The y coordinate of the thumbnail
    int middlex, middley, middlew, middleh;
    //JPEG_NODE *pthumb = headfile;
    //int i, thumbx;

    // space
    space = pfbdev->fb_var.yres / times_space;

    // middle
    middlew = pfbdev->fb_var.xres * times;
    middleh = pfbdev->fb_var.yres * times;
    if (headfile->jpeg_width*times < middlew) 
    {
        middlex = (pfbdev->fb_var.xres - headfile->jpeg_width * times) / 2;
    }
    else 
    {
        middlex = (pfbdev->fb_var.xres - middlew) / 2;
    }
    middley = space;

    //thumbnail
    thumby = 3 * space + middleh;
    thumbh = pfbdev->fb_var.yres - thumby - space;
    //thumbh = pfbdev->fb_var.yres - 4 * space - middleh;
    thumbw = pfbdev->fb_var.xres / (pfbdev->fb_var.yres / thumbh);
    //thumby = pfbdev->fb_var.yres - space - thumbh;
    thumbw_button = thumbw / 3;
    //printf("yres:%d, space:%f, middleh:%d, thumbh:%f, thumby:%f\n", pfbdev->fb_var.yres, space, middleh, thumbh,thumby);
    draw_rectangle(pfbdev, 0, 0, 1024, 768, 0x0000ff00);
    //draw_rectangle(pfbdev, 0, thumby, pfbdev->fb_var.xres-1, space, 0x00ff0000);

    // The background of the thumbnail
    //draw_rectangle(pfbdev, 0, thumby, pfbdev->fb_var.xres-1, thumbh, GRAY);
    draw_rectangle(pfbdev, 0, thumby, pfbdev->fb_var.xres, thumbh, GRAY);
    //draw_rectangle(pfbdev, thumbw_space, thumby, thumbw_button, thumbh, 0x00440000);
    //draw_rectangle(pfbdev, pfbdev->fb_var.xres - thumbw_space - thumbw_button, thumby, thumbw_button, thumbh, 0x00440000);

    //for (i = 0; i < thum_num && headfile != NULL; i++) 
    //{
        ////thumbx = thumbw_space + thumbw_button + thumbw + (thumbw_space + thumbw) * i;
        //thumbx = thumbw_space + thumbw_button + thumbw_space + (thumbw_space + thumbw) * i;
        //display_jpeg(pthumb, pfbdev, thumbx, thumby, thumbw, thumbh);
        //pthumb = pthumb->next;
    //}

    display_jpeg(headfile, pfbdev, middlex, middley, middlew, middleh);
    
    return 0;
}

int jpeg_thumb(PFBDEV pfbdev, JPEG_NODE *headfile, int start, int len) // By XiaoGuo@0601:1911
{
    //int i;
    //JPEG_NODE *pfile = headfile;

    //while ((i != start) && (pfile != NULL)) 
    //{
    //}

    return 0;
}
