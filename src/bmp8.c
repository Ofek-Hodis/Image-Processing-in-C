#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utilities/utilities.c"

t_bmp8* bmp8_loadImage(const char * filename)
{
    FILE* file = fopen(filename, "rb"); //mode rb - read binary (for the bmp8 file)
    if (file == NULL)
    {
        printf("Error - file cannot be opened\n");
        return NULL;
    }
    t_bmp8* img = malloc(sizeof(t_bmp8));
    if (img == NULL)
    {
        printf("Error - failure in memory allocation\n");
        fclose(file);
        return NULL;
    }
    size_t elements_read = fread(img->header, sizeof(unsigned char), 54, file);
    if (!compare_size(elements_read, 54, "failure in reading header"))
    {
        free(img);
        fclose(file);
        return NULL;
    }
    elements_read = fread(img->colorTable, sizeof(unsigned char), 1024, file);
    if (!compare_size(elements_read, 1024, "failure in reading color table"))
    {
        free(img);
        fclose(file);
        return NULL;
    }
    img->width = *(unsigned int *)&(img->header[18]);
    img->height = *(unsigned int *)&(img->header[22]);
    img->colorDepth = *(unsigned short*)&(img->header[28]);
    img->dataSize = *(unsigned int *)&(img->header[34]);

    if (img->dataSize == 0) // DataSize might be automatically set to 0
        img->dataSize = img->width * img->height; //Calculation fit for a grayscale image

    if (img->colorDepth != 8 || img->header[0] != 'B' || img->header[1] != 'M') // Verifying image type
    {
        printf("Error - wrong file type (expected bmp_8)");
        free(img);
        fclose(file);
        return NULL;
    }

    img->data = malloc(img->dataSize*sizeof(unsigned char));
    if (img->data == NULL)
    {
        printf("Error - failure in memory allocation\n");
        free(img);
        fclose(file);
        return NULL;
    }
    elements_read = fread(img->data, sizeof(unsigned char), img->dataSize, file); // Filling the table in image data
    if (!compare_size(elements_read, img->dataSize, "failure to read all data"))
    {
        free(img->data);
        free(img);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return img; // Closing file and returning the initialized image
}

void bmp8_saveImage(const char * filename, t_bmp8 * img)
{
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Error - file cannot be opened\n");
        return;
    }
    size_t elements_read = fwrite(img->header, sizeof(unsigned char), 54, file);
    if (!compare_size(elements_read, 54, "problem writing the header"))
    {
        fclose(file);
        return;
    }
    elements_read = fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    if (!compare_size(elements_read, 1024, "problem writing the color table"))
    {
        fclose(file);
        return;
    }
    elements_read = fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    if (!compare_size(elements_read, img->dataSize, "problem writing the image"))
    {
        fclose(file);
        return;
    }
    fclose(file);
}

void bmp8_free(t_bmp8 * img)
{
    if (img == NULL)
    {
        printf("Error - image empty");
        return;
    }
    free(img->data);
    free(img);
}

void bmp8_printInfo(t_bmp8 * img)
{
    printf("Image Info:\n");
    printf("    Type - bmp8\n");
    printf("    Width:%u\n", img->width);
    printf("    Height:%u\n", img->height);
    printf("    Color Depth:%hu\n", img->colorDepth); //%hu for unsigned short
    printf("    Data Size:%u\n", img->dataSize);
}

void bmp8_negative(t_bmp8 * img)
{
    if (img == NULL)
    {
        printf("Error - image is empty");
        return;
    }
    // modification of the 3 RGB values and skipping the 4th bit (alpha bit)
    for (int i=0; i<1024; i+=4)
    {
        img->colorTable[i] = 255 - img->colorTable[i];
        img->colorTable[i+1] = 255 - img->colorTable[i+1];
        img->colorTable[i+2] = 255 - img->colorTable[i+2];
    }
}

void bmp8_brightness(t_bmp8 * img, int value)
{
    if (img == NULL)
    {
        printf("Error - image is empty");
        return;
    }
    // modification of the 3 RGB values and skipping the 4th bit (alpha bit)
    for (int i=0; i<1024; i+=4)
    {
        int new_val = img->colorTable[i] + value;
        set_pixel_value(img, i, new_val);
        new_val = img->colorTable[i+1] + value;
        set_pixel_value(img, i+1, new_val);
        new_val = img->colorTable[i+2] + value;
        set_pixel_value(img, i+2, new_val);
    }
}

void bmp8_threshold(t_bmp8 * img, int threshold)
{
    if (img == NULL)
    {
        printf("Error - image is empty");
        return;
    }
    //modification of the 3 RGB values and skipping the 4th bit (alpha bit)
    for (int i=0; i<1024; i+=4)
    {
        // Calculating on all pixels so the function can be usable in contexts other than gray scale
        int avg_brightness = (img->colorTable[i] + img->colorTable[i+1] + img->colorTable[i+2])/3;
        if (avg_brightness >= threshold)
        {
            img->colorTable[i] = 255;
            img->colorTable[i+1] = 255;
            img->colorTable[i+2] = 255;
        }
        else
        {
            img->colorTable[i] = 0;
            img->colorTable[i+1] = 0;
            img->colorTable[i+2] = 0;
        }
    }
}

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize) // Add average brightness**
{
    int r = (kernelSize-1)/2; // Used to avoid the borders when applying a filter
    //Temporary stockage to avoid pixels changing and influencing the filter's effect
    unsigned char* temp_data = malloc(img->dataSize*sizeof(unsigned char));
    for (int y=r; y<img->height-r; y++)
    {
        for (int x=r; x<img->width-r; x++)
        {
            float red_sum = 0;
            float green_sum = 0;
            float blue_sum = 0;
            for (int i = r; i>=-r; i--)
            {
                for (int j = r; j>=-r; j--)
                {
                    //Verified all three colour pixels, to generalize the function and to full proof it
                    int pixel_index = img->data[(y + i) * img->width + x + j];
                    int table_position = 4*pixel_index;
                    red_sum += kernel[r - i][r - j] * img->colorTable[table_position];
                    green_sum += kernel[r - i][r - j] * img->colorTable[1+table_position];
                    blue_sum += kernel[r - i][r - j] * img->colorTable[2+table_position];
                }
            }
            //Division by three done as late as possible to avoid rounding errors
            int total_brightness = (int)((red_sum+green_sum+blue_sum)/3);
            temp_data[y * img->width + x] = is_in_8bit_range(total_brightness);
        }
    }

    for (int y=0; y<r; y++)//Refilling the borders of temp_data
    {
        for (int x=0; x<img->width; x++)
        {
            temp_data[y*img->width+x] = img->data[y*img->width+x];
        }
    }
    for (int y=(int)img->height-r; y<img->height; y++)
    {
        for (int x=0; x<(int)img->width; x++)
        {
            temp_data[y*img->width+x] = img->data[y*img->width+x];
        }
    }
    for (int y=r; y<(int)img->height - r; y++)
    {
        for (int x=0; x<r; x++)
        {
            temp_data[y*img->width+x] = img->data[y*img->width+x];
        }
    }
    for (int y=r; y<(int)img->height - r; y++)
    {
        for (int x=(int)img->width-r; x<img->width; x++)
        {
            temp_data[y*img->width+x] = img->data[y*img->width+x];
        }
    }

    free(img->data);
    img->data = temp_data;
}
}

