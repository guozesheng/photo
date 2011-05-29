#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

int fb_init(PFBDEV pFbdev)
{
    if ((pFbdev->fb = open(pFbdev->dev, O_RDWR)) < 0)
    {
        perror("open dev");
        return -1;
    }

    if (ioctl(pFbdev->fb, FBIOGET_VSCREENINFO, &(pFbdev->fb_var)) == -1)
    {
        perror("ioctl");
        return -1;
    }

    pFbdev->memo = mmap(NULL, pFbdev->fb_var.xres * pFbdev->fb_var.yres * pFbdev->fb_var.bits_per_pixel / 8, PROT_READ | PROT_WRITE, MAP_SHARED, pFbdev->fb, 0);
    
    return 0;
}

int fb_destroy(PFBDEV pfbdev)
{
    close(pfbdev->fb);
    pfbdev->fb = -1;

    munmap(pfbdev->memo, pfbdev->fb_var.xres * pfbdev->fb_var.yres * pfbdev->fb_var.bits_per_pixel / 8);
    
    return 0;
}

int fb_memset(void *addr, int c, size_t len)
{
    memset(addr, c, len);

    return 0;
}
