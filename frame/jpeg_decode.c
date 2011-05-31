#include <stdio.h>
#include <jpeglib.h>
#include <jerror.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include "main.h"

int rgb24to32(u8_t *buf24, u32_t *buf, JDIMENSION jpeg_width, JDIMENSION jpeg_height)
{
    int i;

    for(i = 0; i < jpeg_width * jpeg_height; i++)
    {
        *((u8_t *)&buf[i] + 2) = buf24[i * 3 + 0]; 
        *((u8_t *)&buf[i] + 1) = buf24[i * 3 + 1]; 
        *((u8_t *)&buf[i] + 0) = buf24[i * 3 + 2]; 
        *((u8_t *)&buf[i] + 3) = 0x00;
    }   

    return 0;
}

int rgb24to16(u8_t *buf24, u16_t *buf, JDIMENSION jpeg_width, JDIMENSION jpeg_height)
{
    //u16_t *buf16 = NULL;
	
	u32_t r24,g24,b24;
	u32_t r16,g16,b16;
	u16_t rgb16;
	int i;

	//if ((buf16 = malloc (jpeg_inf.w * jpeg_inf.h * 2)) == NULL)
		//return -1;

	for (i = 0 ;i < jpeg_width * jpeg_height ; i++)	{

		r24 = buf24[i * 3 + 0];
		g24 = buf24[i * 3 + 1];
		b24 = buf24[i * 3 + 2];	

		b16 = (b24 * 0x1f) / 0xff;
		g16 = (g24 * 0x3f) / 0xff;
		r16 = (r24 * 0x1f) / 0xff;
	
		rgb16 = b16 | (g16 <<5) | (r16 <<11);
	
		buf[i] = rgb16;
	}
	return 0;
}

JPEG_NODE *jpeg_decode(PFBDEV pfbdev, JPEG_NODE *p, const char *img_file)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *infile;
    u8_t *buffer;
    unsigned int y;
    register JSAMPARRAY lineBuf;

    // open jpeg file
    if ((infile = fopen(img_file, "rb")) == NULL)
    {
        perror("img open");
        return NULL;
    }

    // init jpeg decompress object error handler
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);


    // bind jpeg decompress object to infile
    jpeg_stdio_src(&cinfo, infile);

    // read jpeg header
    jpeg_read_header(&cinfo, TRUE);

    /*
     * decompress process.
     * note: after jpeg_start_decompress() is called.
     * the dimension infomation will be known
     * so allocate memory buffer for scanline immediately
     */
    jpeg_start_decompress(&cinfo);
    //if ((cinfo.output_width > pfbdev->fb_var.xres) || (cinfo.output_height > pfbdev->fb_var.yres)) 
    //{
        //printf("Too large JPEG file, cannot display\n");
        //return NULL;
    //}
    lineBuf = cinfo.mem->alloc_sarray ((j_common_ptr) &cinfo, JPOOL_IMAGE, (cinfo.output_width * cinfo.output_components), 1);
    buffer = (u8_t *)malloc(3 * cinfo.output_width * cinfo.output_height);
    if (buffer == NULL) 
    {
        perror("buffer");
        return NULL;
    }

    if (cinfo.output_components == 3) 
    {
        for (y = 0; y < cinfo.output_height; y++) 
        {
            jpeg_read_scanlines(&cinfo, lineBuf, 1);
            memcpy((buffer + y * cinfo.output_width * 3), lineBuf[0], 3 * cinfo.output_width);
        }
    }
    else if (cinfo.output_components == 1) 
    {
        unsigned int col;
        int lineOffset = (p->jpeg_width * 3); 
        int lineBufIndex;
        int x; 
        int y;

        for (y = 0; y < cinfo.output_height; y++)
        {
            jpeg_read_scanlines (&cinfo, lineBuf, 1); 

            lineBufIndex = 0;
            for (x = 0; x < lineOffset; x++) 
            {
                col = lineBuf[0][lineBufIndex];

                buffer[(lineOffset * y) + x] = col;
                x++;
                buffer[(lineOffset * y) + x] = col;
                x++;
                buffer[(lineOffset * y) + x] = col;

                lineBufIndex++;
            }    
        }   
    }
    else 
    {
        return NULL;
    }

    // finish decompress, destroy decompress object
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    // the flow is only for TEST
    
    void *buf = malloc(cinfo.output_width * cinfo.output_height * pfbdev->fb_var.bits_per_pixel / 8); 

    if (pfbdev->fb_var.bits_per_pixel == 32) 
    {
        rgb24to32(buffer, buf, cinfo.output_width, cinfo.output_height);
    }
    else if (pfbdev->fb_var.bits_per_pixel == 16) 
    {
        rgb24to16(buffer, buf, cinfo.output_width, cinfo.output_height);
    }
    else 
    {
        return NULL;
    }

    free(buffer);

    p->pjpeg = buf;

    // by XiaoGuo
    p->jpeg_width = cinfo.output_width;
    p->jpeg_height = cinfo.output_height;

    // End of the TEST

    fclose(infile);
    return p;
}

