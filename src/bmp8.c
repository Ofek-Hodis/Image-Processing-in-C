#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>

//Size comparison function used (for example) in verification of successful reading or writing
int compare_size(const size_t size1, const size_t size2, const char* error_message)
{
    if (size1 == size2) return 1;
    printf("Error - %s\n", error_message);
    return 0;
}

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

    if (img->colorDepth != 8 || img->header[0] == 'B' || img->header[1] == 'M') // Verifying image type
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

